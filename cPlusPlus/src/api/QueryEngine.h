#ifndef QUERY_ENGINE_H
#define QUERY_ENGINE_H
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <iostream>
#include <regex>
#include "../data/CSVReader.cpp"

class QueryEngine {
private:
    std::vector<CSVRow> data;

    bool evaluateCondition(const CSVRow& row, const std::string& condition) {
        std::regex pattern(R"((\S+)\s*([=<>!]+)\s*(.+))");
        std::smatch matches;
        if (std::regex_match(condition, matches, pattern)) {
            std::string column = matches[1];
            std::string op = matches[2];
            std::string value = matches[3];

            // Remove quotes if present
            if (value.front() == '"' && value.back() == '"') {
                value = value.substr(1, value.length() - 2);
            }

            std::string rowValue = row.getValueByColumnName(column);

            if (op == "=") return rowValue == value;
            if (op == ">") return std::stod(rowValue) > std::stod(value);
            if (op == "<") return std::stod(rowValue) < std::stod(value);
            if (op == ">=") return std::stod(rowValue) >= std::stod(value);
            if (op == "<=") return std::stod(rowValue) <= std::stod(value);
            if (op == "!=") return rowValue != value;
        }
        return false;
    }

public:
    QueryEngine(const std::string& filename) {
        CSVReader reader(filename);
        data = reader.readCSV();
    }

    std::vector<std::unordered_map<std::string, std::string>> executeQuery(
        const std::vector<std::string>& selectColumns,
        const std::vector<std::string>& conditions,
        const std::string& orderBy,
        const std::string& limit
    ) {
        std::vector<std::unordered_map<std::string, std::string>> results;

        for (const auto& row : data) {
            bool satisfiesConditions = conditions.empty();
            for (const auto& condition : conditions) {
                if (evaluateCondition(row, condition)) {
                    satisfiesConditions = true;
                    break;
                }
            }

            if (satisfiesConditions) {
                std::unordered_map<std::string, std::string> resultRow;
                for (const auto& col : selectColumns) {
                    resultRow[col] = row.getValueByColumnName(col);
                }
                results.push_back(resultRow);
            }
        }

        // Implement ordering
        if (!orderBy.empty()) {
            std::istringstream iss(orderBy);
            std::string column, order;
            iss >> column >> order;

            std::sort(results.begin(), results.end(), [&](const auto& a, const auto& b) {
                if (order == "DESC") {
                    return a.at(column) > b.at(column);
                }
                return a.at(column) < b.at(column);
            });
        }

        // Implement limit
        if (!limit.empty()) {
            int limitValue = std::stoi(limit);
            if (limitValue < results.size()) {
                results.resize(limitValue);
            }
        }

        return results;
    }
};

#endif