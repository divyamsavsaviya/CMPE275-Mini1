#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <algorithm>
#include <stdexcept>

class ColumnStore {
public:
    using ColumnData = std::variant<std::vector<std::string>, std::vector<int>, std::vector<float>>;

private:
    std::unordered_map<std::string, ColumnData> columns;

    template<typename T>
    const std::vector<T>& getTypedColumn(const std::string& columnName) const {
        auto it = columns.find(columnName);
        if (it == columns.end()) {
            throw std::out_of_range("Column not found: " + columnName);
        }
        return std::get<std::vector<T>>(it->second);
    }

public:
    void addColumn(const std::string& columnName, const ColumnData& data) {
        columns[columnName] = data;
    }

    const std::vector<std::string>& getStringColumn(const std::string& columnName) const {
        return getTypedColumn<std::string>(columnName);
    }

    const std::vector<int>& getIntColumn(const std::string& columnName) const {
        return getTypedColumn<int>(columnName);
    }

    const std::vector<float>& getFloatColumn(const std::string& columnName) const {
        return getTypedColumn<float>(columnName);
    }

    bool isNumericColumn(const std::string& columnName) const {
        auto it = columns.find(columnName);
        if (it == columns.end()) {
            return false;
        }
        return std::holds_alternative<std::vector<int>>(it->second) || 
               std::holds_alternative<std::vector<float>>(it->second);
    }

    size_t getRowCount() const {
        if (columns.empty()) return 0;
        return std::visit([](const auto& vec) { return vec.size(); }, columns.begin()->second);
    }

    const std::unordered_map<std::string, ColumnData>& getColumns() const {
        return columns;
    }
};
