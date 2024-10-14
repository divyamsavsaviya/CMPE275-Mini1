#include "QueryEngine.h"
#include <algorithm>
#include <execution>
#include <immintrin.h>
#include <fstream>
#include <sstream>
#include <omp.h>
#include <iostream>
#include "PerformanceMeasurement.h"

QueryEngine::QueryEngine()
    : threadPool(std::thread::hardware_concurrency()) {
    // Initialize with empty data
}

QueryEngine::QueryEngine(const std::string& filename)
    : dataApi(DataApi(filename)), threadPool(std::thread::hardware_concurrency()), csvReader(CSVReader(filename)) {
    initializeFromFile(filename);
    std::cout << "Available columns:" << std::endl;
    for (const auto& column : dataStore.getColumns()) {
        std::cout << "'" << column.first << "'" << std::endl;
    }
}

void QueryEngine::initializeFromFile(const std::string& filename) {
    if (!csvReader) {
        csvReader.emplace(filename);
    }
    std::vector<std::unordered_map<std::string, std::string>> csvData = csvReader->readCSV();

    std::cout << "CSV Data size: " << csvData.size() << std::endl;

    if (!csvData.empty()) {
        std::vector<std::string> headers;
        for (const auto& pair : csvData[0]) {
            headers.push_back(pair.first);
            dataStore.addColumn(pair.first, std::vector<std::string>());
        }

        std::cout << "Number of columns: " << headers.size() << std::endl;

        for (size_t i = 0; i < csvData.size(); ++i) {
            for (const auto& header : headers) {
                auto it = csvData[i].find(header);
                if (it != csvData[i].end()) {
                    dataStore.getColumn(header).push_back(it->second);
                } else {
                    dataStore.getColumn(header).push_back("");
                }
            }
        }

        std::cout << "Number of rows: " << dataStore.getColumn(headers[0]).size() << std::endl;
    } else {
        std::cout << "CSV Data is empty!" << std::endl;
    }
}

QueryEngine::~QueryEngine() {
    // Destructor implementation (if needed)
}

std::pair<std::vector<std::unordered_map<std::string, std::string>>, PerformanceMeasurement> 
QueryEngine::executeQueryWithPerformance(const std::vector<std::string>& selectColumns,
                                         const std::vector<std::pair<std::string, std::string>>& conditions,
                                         const std::string& orderBy,
                                         int limit) {
    PerformanceMeasurement perf;
    perf.start();

    auto results = executeQueryParallel(selectColumns, conditions, orderBy, limit);

    perf.stop();
    return {results, perf};
}

std::vector<std::unordered_map<std::string, std::string>> 
QueryEngine::filterData(const std::vector<std::unordered_map<std::string, std::string>>& data,
                        const std::vector<std::pair<std::string, std::string>>& conditions) {
    std::vector<std::unordered_map<std::string, std::string>> result;
    result.reserve(data.size());

    for (const auto& condition : conditions) {
        const auto& column = condition.first;
        const auto& value = condition.second;

        std::vector<float> numericValues;
        numericValues.reserve(data.size());

        for (const auto& row : data) {
            auto it = row.find(column);
            if (it != row.end()) {
                numericValues.push_back(std::stof(it->second));
            }
        }

        std::vector<int> matchResults(data.size());
        float threshold = std::stof(value);

        vectorized_filter(numericValues.data(), matchResults.data(), numericValues.size(), threshold);

        for (size_t i = 0; i < data.size(); ++i) {
            if (matchResults[i]) {
                result.push_back(data[i]);
            }
        }
    }

    return result;
}

void vectorized_filter(const float* data, int* result, int size, float threshold) {
    __m256 thresh_vec = _mm256_set1_ps(threshold);
    __m256i ones = _mm256_set1_epi32(1);
    __m256i zeros = _mm256_setzero_si256();

    for (int i = 0; i < size; i += 8) {
        __m256 data_vec = _mm256_loadu_ps(data + i);
        __m256 cmp_result = _mm256_cmp_ps(data_vec, thresh_vec, _CMP_GT_OQ);
        __m256i int_result = _mm256_blendv_epi8(zeros, ones, _mm256_castps_si256(cmp_result));
        _mm256_storeu_si256((__m256i*)(result + i), int_result);
    }

    // Handle any remaining elements
    for (int i = (size / 8) * 8; i < size; ++i) {
        result[i] = (data[i] > threshold) ? 1 : 0;
    }
}

void QueryEngine::sortData(std::vector<std::unordered_map<std::string, std::string>>& data,
                           const std::string& orderBy) {
    std::sort(std::execution::par_unseq, data.begin(), data.end(),
        [&orderBy](const auto& a, const auto& b) {
            auto it_a = a.find(orderBy);
            auto it_b = b.find(orderBy);
            if (it_a != a.end() && it_b != b.end()) {
                return it_a->second < it_b->second;
            }
            return false;
        });
}

std::vector<std::unordered_map<std::string, std::string>>
QueryEngine::projectData(const std::vector<std::unordered_map<std::string, std::string>>& data,
                         const std::vector<std::string>& selectColumns) {
    std::vector<std::unordered_map<std::string, std::string>> result;
    result.reserve(data.size());

    for (const auto& row : data) {
        std::unordered_map<std::string, std::string> projectedRow;
        for (const auto& col : selectColumns) {
            auto it = row.find(col);
            if (it != row.end()) {
                projectedRow[col] = it->second;
            }
        }
        result.push_back(std::move(projectedRow));
    }

    return result;
}


std::vector<std::unordered_map<std::string, std::string>> 
QueryEngine::executeQueryParallel(const std::vector<std::string>& selectColumns,
                                  const std::vector<std::pair<std::string, std::string>>& conditions,
                                  const std::string& orderBy,
                                  int limit) {
    std::vector<std::unordered_map<std::string, std::string>> results;
    size_t rowCount = dataStore.getRowCount();

    std::cout << "Total rows: " << rowCount << std::endl;
    std::cout << "Select columns: ";
    for (const auto& col : selectColumns) {
        std::cout << col << " ";
    }
    std::cout << std::endl;

    std::cout << "Conditions: ";
    for (const auto& cond : conditions) {
        std::cout << cond.first << "=" << cond.second << " ";
    }
    std::cout << std::endl;

    // Create a vector of futures to store the results of each thread
    std::vector<std::future<std::vector<std::unordered_map<std::string, std::string>>>> futures;

    // Determine the number of threads to use
    size_t numThreads = std::thread::hardware_concurrency();
    size_t rowsPerThread = rowCount / numThreads;

    auto removeQuotes = [](const std::string& str) {
        std::string result = str;
        result.erase(std::remove(result.begin(), result.end(), '"'), result.end());
        return result;
    };

    for (size_t i = 0; i < numThreads; ++i) {
        size_t startRow = i * rowsPerThread;
        size_t endRow = (i == numThreads - 1) ? rowCount : (i + 1) * rowsPerThread;

        futures.push_back(threadPool.enqueue([this, &selectColumns, &conditions, startRow, endRow, removeQuotes]() {
            std::vector<std::unordered_map<std::string, std::string>> threadResults;

            for (size_t row = startRow; row < endRow; ++row) {
                bool rowMatches = true;

                for (const auto& condition : conditions) {
                    try {
                        const auto& columnData = dataStore.getColumn(removeQuotes(condition.first));
                        if (row >= columnData.size() || columnData[row] != removeQuotes(condition.second)) {
                            rowMatches = false;
                            break;
                        }
                    } catch (const std::out_of_range& e) {
                        std::cerr << "Error: Column '" << removeQuotes(condition.first) << "' not found. Available columns are:" << std::endl;
                        for (const auto& column : dataStore.getColumns()) {
                            std::cerr << "'" << column.first << "'" << std::endl;
                        }
                        rowMatches = false;
                        break;
                    }
                }

                if (rowMatches) {
                    std::unordered_map<std::string, std::string> resultRow;
                    for (const auto& column : selectColumns) {
                        try {
                            const auto& columnData = dataStore.getColumn(removeQuotes(column));
                            if (row < columnData.size()) {
                                resultRow[removeQuotes(column)] = columnData[row];
                            }
                        } catch (const std::out_of_range& e) {
                            std::cerr << "Error: Column '" << removeQuotes(column) << "' not found." << std::endl;
                        }
                    }
                    threadResults.push_back(std::move(resultRow));
                }
            }

            return threadResults;
        }));
    }

    // Collect results from all threads
    for (auto& future : futures) {
        auto threadResults = future.get();
        results.insert(results.end(), threadResults.begin(), threadResults.end());
    }

    std::cout << "Results before ordering: " << results.size() << std::endl;

    // Apply ordering if specified
    if (!orderBy.empty()) {
        std::sort(results.begin(), results.end(),
            [&orderBy](const auto& a, const auto& b) {
                auto it_a = a.find(orderBy);
                auto it_b = b.find(orderBy);
                if (it_a != a.end() && it_b != b.end()) {
                    return it_a->second < it_b->second;
                }
                return false;
            });
    }

    // Apply limit if specified
    if (limit > 0 && static_cast<size_t>(limit) < results.size()) {
        results.resize(limit);
    }

    std::cout << "Final results: " << results.size() << std::endl;

    return results;
}

std::vector<std::unordered_map<std::string, std::string>> 
QueryEngine::processChunk(const std::vector<std::unordered_map<std::string, std::string>>& chunk,
                          const std::vector<std::string>& selectColumns,
                          const std::vector<std::pair<std::string, std::string>>& conditions) {
    auto filteredChunk = filterData(chunk, conditions);
    return projectData(filteredChunk, selectColumns);
}

std::vector<std::unordered_map<std::string, std::string>> 
QueryEngine::executeQuery(const std::vector<std::string>& selectColumns,
                          const std::vector<std::pair<std::string, std::string>>& conditions,
                          const std::string& orderBy,
                          int limit) {
    std::vector<std::unordered_map<std::string, std::string>> results;
    size_t rowCount = dataStore.getRowCount();

    std::cout << "Executing query with " << selectColumns.size() << " columns and " 
              << conditions.size() << " conditions." << std::endl;
    std::cout << "Total rows in dataStore: " << rowCount << std::endl;

    #pragma omp parallel
    {
        std::vector<std::unordered_map<std::string, std::string>> localResults;

        #pragma omp for nowait
        for (size_t i = 0; i < rowCount; ++i) {
            bool rowMatches = true;
            for (const auto& condition : conditions) {
                const auto& column = dataStore.getColumn(condition.first);
                if (column[i].empty()) {
                    rowMatches = false;
                    break;
                }
                
                if (std::isdigit(column[i][0]) || (column[i][0] == '-' && column[i].size() > 1 && std::isdigit(column[i][1]))) {
                    // Numeric comparison
                    double columnValue = std::stod(column[i]);
                    double conditionValue = std::stod(condition.second);
                    if (!(columnValue == conditionValue)) {
                        rowMatches = false;
                        break;
                    }
                } else {
                    // String comparison
                    if (column[i] != condition.second) {
                        rowMatches = false;
                        break;
                    }
                }
            }

            if (rowMatches) {
                std::unordered_map<std::string, std::string> row;
                for (const auto& col : selectColumns) {
                    row[col] = dataStore.getColumn(col)[i];
                }
                localResults.push_back(std::move(row));
            }
        }

        #pragma omp critical
        {
            results.insert(results.end(), localResults.begin(), localResults.end());
        }
    }

    if (!orderBy.empty()) {
        std::sort(results.begin(), results.end(),
            [&orderBy](const auto& a, const auto& b) {
                return a.at(orderBy) < b.at(orderBy);
            });
    }

    if (limit > 0 && static_cast<size_t>(limit) < results.size()) {
        results.resize(limit);
    }

    std::cout << "Found " << results.size() << " matching rows." << std::endl;

    return results;
}

// Implement these methods for MPI data transfer
void QueryEngine::serializeData(std::vector<char>& buffer) const {
    std::ostringstream oss;
    
    // Serialize the number of columns
    size_t columnCount = dataStore.getColumnCount();
    oss.write(reinterpret_cast<const char*>(&columnCount), sizeof(columnCount));
    
    // Serialize each column
    for (const auto& columnPair : dataStore.getColumns()) {
        const std::string& columnName = columnPair.first;
        const std::vector<std::string>& columnData = columnPair.second;
        
        // Serialize column name
        size_t nameSize = columnName.size();
        oss.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
        oss.write(columnName.c_str(), nameSize);
        
        // Serialize column data
        size_t dataSize = columnData.size();
        oss.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));
        for (const auto& value : columnData) {
            size_t valueSize = value.size();
            oss.write(reinterpret_cast<const char*>(&valueSize), sizeof(valueSize));
            oss.write(value.c_str(), valueSize);
        }
    }
    
    // Convert the string stream to a vector of chars
    std::string data = oss.str();
    buffer.assign(data.begin(), data.end());
}

void QueryEngine::deserializeData(const std::vector<char>& buffer) {
    std::istringstream iss(std::string(buffer.begin(), buffer.end()));
    
    // Clear existing data
    dataStore = ColumnStore();
    
    // Deserialize the number of columns
    size_t columnCount;
    iss.read(reinterpret_cast<char*>(&columnCount), sizeof(columnCount));
    
    // Deserialize each column
    for (size_t i = 0; i < columnCount; ++i) {
        // Deserialize column name
        size_t nameSize;
        iss.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
        std::string columnName(nameSize, '\0');
        iss.read(&columnName[0], nameSize);
        
        // Deserialize column data
        size_t dataSize;
        iss.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));
        std::vector<std::string> columnData;
        columnData.reserve(dataSize);
        
        for (size_t j = 0; j < dataSize; ++j) {
            size_t valueSize;
            iss.read(reinterpret_cast<char*>(&valueSize), sizeof(valueSize));
            std::string value(valueSize, '\0');
            iss.read(&value[0], valueSize);
            columnData.push_back(std::move(value));
        }
        
        dataStore.addColumn(columnName, std::move(columnData));
    }
}