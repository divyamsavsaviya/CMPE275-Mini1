#include "QueryEngine.h"
#include <algorithm>
#include <stdexcept>
#include <iostream> 

QueryEngine::QueryEngine(const std::string& filename) {
    CSVReader reader(filename);
    data = reader.readCSV();
    std::cout << "Loaded " << data.size() << " rows of data." << std::endl;
    
    if (!data.empty()) {
        std::cout << "Available columns: ";
        for (const auto& [key, value] : data[0]) {
            std::cout << key << " ";
        }
        std::cout << std::endl;
    }
}

std::string removeQuotes(const std::string& str) {
    if (str.size() >= 2 && str.front() == '"' && str.back() == '"') {
        return str.substr(1, str.size() - 2);
    }
    return str;
}

std::vector<std::unordered_map<std::string, std::string>> QueryEngine::executeQuery(
    const std::vector<std::string>& selectColumns,
    const std::vector<std::pair<std::string, std::string>>& conditions,
    const std::string& orderBy,
    int limit
) {
    std::cout << "Executing query with " << selectColumns.size() << " columns and " << conditions.size() << " conditions." << std::endl;

    std::vector<std::unordered_map<std::string, std::string>> results;

    for (const auto& row : data) {
        bool satisfiesConditions = true;
        for (const auto& [column, value] : conditions) {
            auto it = row.find('"' + column + '"');
            if (it == row.end()) {
                std::cout << "Condition column '" << column << "' not found in row." << std::endl;
                satisfiesConditions = false;
                break;
            }
            if (removeQuotes(it->second) != value) {
                satisfiesConditions = false;
                break;
            }
        }

        if (satisfiesConditions) {
            std::unordered_map<std::string, std::string> resultRow;
            for (const auto& col : selectColumns) {
                auto it = row.find('"' + col + '"');
                if (it != row.end()) {
                    resultRow[col] = removeQuotes(it->second);
                } else {
                    std::cout << "Selected column '" << col << "' not found in row." << std::endl;
                }
            }
            if (!resultRow.empty()) {
                results.push_back(resultRow);
            }
        }
    }

    std::cout << "Found " << results.size() << " matching rows." << std::endl;

    if (!orderBy.empty()) {
        std::sort(results.begin(), results.end(),
            [&orderBy](const auto& a, const auto& b) {
                auto it_a = a.find(orderBy);
                auto it_b = b.find(orderBy);
                if (it_a == a.end() || it_b == b.end()) {
                    return false;
                }
                return it_a->second < it_b->second;
            });
    }

    if (limit > 0 && limit < static_cast<int>(results.size())) {
        results.resize(limit);
    }

    return results;
}