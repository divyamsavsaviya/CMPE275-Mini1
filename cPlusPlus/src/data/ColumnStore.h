#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm> 
#include <cctype>   

class ColumnStore {
private:
    std::unordered_map<std::string, std::vector<std::string>> columns;
    size_t rowCount;

public:
    void addColumn(const std::string& columnName, std::vector<std::string>&& data) {
        columns[columnName] = std::move(data);
        rowCount = columns[columnName].size();
    }

    std::vector<std::string>& getColumn(const std::string& columnName) {
        return columns.at(columnName);
    }

    const std::vector<std::string>& getColumn(const std::string& columnName) const {
        auto it = std::find_if(columns.begin(), columns.end(),
            [&columnName](const auto& pair) {
                return std::equal(columnName.begin(), columnName.end(),
                                  pair.first.begin(), pair.first.end(),
                                  [](char a, char b) {
                                      return std::tolower(static_cast<unsigned char>(a)) == std::tolower(static_cast<unsigned char>(b));
                                  });
            });
        if (it == columns.end()) {
            throw std::out_of_range("Column not found: " + columnName);
        }
        return it->second;
    }

    size_t getRowCount() const {
        if (columns.empty()) return 0;
        return columns.begin()->second.size();
    }

    size_t getColumnCount() const { return columns.size(); }
    const std::unordered_map<std::string, std::vector<std::string>>& getColumns() const { return columns; }
};
