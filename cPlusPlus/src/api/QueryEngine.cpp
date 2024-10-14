#include "QueryEngine.h"
#include <algorithm>
#include <execution>
#include <immintrin.h>

QueryEngine::QueryEngine(const std::string& filename, int, char**)
    : dataApi(filename), threadPool(std::thread::hardware_concurrency()) {
    // Initialize MPI or any other setup if needed
}

QueryEngine::~QueryEngine() {
    // Add any necessary cleanup
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
    auto allData = dataApi.getAllData();
    const size_t num_threads = std::thread::hardware_concurrency();
    size_t chunkSize = allData.size() / num_threads;

    std::vector<std::future<std::vector<std::unordered_map<std::string, std::string>>>> futures;

    for (size_t i = 0; i < allData.size(); i += chunkSize) {
        size_t end = std::min(i + chunkSize, allData.size());
        std::vector<std::unordered_map<std::string, std::string>> chunk(allData.begin() + i, allData.begin() + end);

        futures.push_back(threadPool.enqueue([this, chunk, &selectColumns, &conditions]() {
            return processChunk(chunk, selectColumns, conditions);
        }));
    }

    std::vector<std::unordered_map<std::string, std::string>> results;
    for (auto& future : futures) {
        auto chunkResult = future.get();
        results.insert(results.end(), chunkResult.begin(), chunkResult.end());
    }

    if (!orderBy.empty()) {
        sortData(results, orderBy);
    }

    if (limit > 0 && limit < static_cast<int>(results.size())) {
        results.resize(limit);
    }

    return projectData(results, selectColumns);
}

std::vector<std::unordered_map<std::string, std::string>> 
QueryEngine::processChunk(const std::vector<std::unordered_map<std::string, std::string>>& chunk,
                          const std::vector<std::string>& selectColumns,
                          const std::vector<std::pair<std::string, std::string>>& conditions) {
    auto filteredChunk = filterData(chunk, conditions);
    return projectData(filteredChunk, selectColumns);
}
