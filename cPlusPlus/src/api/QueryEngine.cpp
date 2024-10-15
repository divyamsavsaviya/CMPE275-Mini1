#include "QueryEngine.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include "PerformanceMeasurement.h"

QueryEngine::QueryEngine() : threadPool(std::thread::hardware_concurrency()) {
}

QueryEngine::QueryEngine(const std::string& filename)
    : threadPool(std::thread::hardware_concurrency()),
      csvReader(std::make_unique<CSVReader>(filename)) {
    initializeFromFile(filename);
}

void QueryEngine::initializeFromFile(const std::string& filename) {
    if (!csvReader) {
        csvReader = std::make_unique<CSVReader>(filename);
    }
    dataStore = csvReader->readCSV();

    std::cout << "Number of columns: " << dataStore.getColumns().size() << std::endl;
    std::cout << "Number of rows: " << dataStore.getRowCount() << std::endl;
}

std::vector<std::unordered_map<std::string, std::string>> QueryEngine::executeQuery(
    const std::vector<std::string>& selectColumns,
    const std::vector<std::pair<std::string, std::string>>& conditions,
    const std::string& orderBy,
    int limit) {
    // Check if all requested columns exist
    for (const auto& col : selectColumns) {
        if (dataStore.getColumns().find(col) == dataStore.getColumns().end()) {
            throw std::runtime_error("Column not found: " + col);
        }
    }

    std::vector<std::unordered_map<std::string, std::string>> results;
    size_t rowCount = dataStore.getRowCount();

    auto compareValues = [this](const std::string& columnName, const std::string& value, size_t row) {
        if (dataStore.isNumericColumn(columnName)) {
            float numericValue = std::stof(value);
            if (std::holds_alternative<std::vector<int>>(dataStore.getColumns().at(columnName))) {
                int columnValue = std::get<std::vector<int>>(dataStore.getColumns().at(columnName))[row];
                return columnValue == numericValue;
            } else {
                float columnValue = std::get<std::vector<float>>(dataStore.getColumns().at(columnName))[row];
                return std::abs(columnValue - numericValue) < 1e-6;
            }
        } else {
            const auto& columnValue = std::get<std::vector<std::string>>(dataStore.getColumns().at(columnName))[row];
            return columnValue == value;
        }
    };

    for (size_t row = 0; row < rowCount; ++row) {
        bool rowMatches = true;
        for (const auto& condition : conditions) {
            if (!compareValues(condition.first, condition.second, row)) {
                rowMatches = false;
                break;
            }
        }

        if (rowMatches) {
            std::unordered_map<std::string, std::string> resultRow;
            for (const auto& column : selectColumns) {
                const auto& columnData = dataStore.getColumns().at(column);
                if (std::holds_alternative<std::vector<std::string>>(columnData)) {
                    resultRow[column] = std::get<std::vector<std::string>>(columnData)[row];
                } else if (std::holds_alternative<std::vector<int>>(columnData)) {
                    resultRow[column] = std::to_string(std::get<std::vector<int>>(columnData)[row]);
                } else {
                    resultRow[column] = std::to_string(std::get<std::vector<float>>(columnData)[row]);
                }
            }
            results.push_back(std::move(resultRow));
        }
    }

    // Sorting
    if (!orderBy.empty()) {
        if (dataStore.getColumns().find(orderBy) == dataStore.getColumns().end()) {
            throw std::runtime_error("Order by column not found: " + orderBy);
        }

        std::sort(results.begin(), results.end(), [&](const auto& a, const auto& b) {
            auto& columnData = dataStore.getColumns().at(orderBy);
            if (std::holds_alternative<std::vector<int>>(columnData)) {
                int aVal = 0, bVal = 0;
                try {
                    aVal = std::stoi(a.at(orderBy));
                } catch (const std::exception&) {}
                try {
                    bVal = std::stoi(b.at(orderBy));
                } catch (const std::exception&) {}
                return aVal < bVal;
            } else if (std::holds_alternative<std::vector<float>>(columnData)) {
                float aVal = 0.0f, bVal = 0.0f;
                try {
                    aVal = std::stof(a.at(orderBy));
                } catch (const std::exception&) {}
                try {
                    bVal = std::stof(b.at(orderBy));
                } catch (const std::exception&) {}
                return aVal < bVal;
            } else {
                return a.at(orderBy) < b.at(orderBy);
            }
        });
        if (orderBy.find("DESC") != std::string::npos) {
            std::reverse(results.begin(), results.end());
        }
    }

    if (limit > 0 && static_cast<size_t>(limit) < results.size()) {
        results.resize(limit);
    }

    return results;
}

void QueryEngine::serializeData(std::vector<char>& buffer) const {
    std::ostringstream oss;
    
    size_t columnCount = dataStore.getColumns().size();
    oss.write(reinterpret_cast<const char*>(&columnCount), sizeof(columnCount));
    
    for (const auto& [columnName, columnData] : dataStore.getColumns()) {
        size_t nameSize = columnName.size();
        oss.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
        oss.write(columnName.c_str(), nameSize);
        
        size_t dataSize = std::visit([](const auto& vec) { return vec.size(); }, columnData);
        oss.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));
        
        std::visit([&oss](const auto& vec) {
            for (const auto& value : vec) {
                oss.write(reinterpret_cast<const char*>(&value), sizeof(value));
            }
        }, columnData);
    }
    
    std::string data = oss.str();
    buffer.assign(data.begin(), data.end());
}

void QueryEngine::deserializeData(const std::vector<char>& buffer) {
    std::istringstream iss(std::string(buffer.begin(), buffer.end()));
    
    dataStore = ColumnStore();
    
    size_t columnCount;
    iss.read(reinterpret_cast<char*>(&columnCount), sizeof(columnCount));
    
    for (size_t i = 0; i < columnCount; ++i) {
        size_t nameSize;
        iss.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
        std::string columnName(nameSize, '\0');
        iss.read(&columnName[0], nameSize);
        
        size_t dataSize;
        iss.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));
        
        // For simplicity, we'll assume all columns are strings in this example
        std::vector<std::string> columnData(dataSize);
        for (size_t j = 0; j < dataSize; ++j) {
            size_t valueSize;
            iss.read(reinterpret_cast<char*>(&valueSize), sizeof(valueSize));
            std::string value(valueSize, '\0');
            iss.read(&value[0], valueSize);
            columnData[j] = std::move(value);
        }
        
        dataStore.addColumn(columnName, columnData);
    }
}

std::vector<std::string> QueryEngine::getColumnNames() const {
    std::vector<std::string> columnNames;
    for (const auto& [colName, colData] : dataStore.getColumns()) {
        columnNames.push_back(colName);
    }
    return columnNames;
}
