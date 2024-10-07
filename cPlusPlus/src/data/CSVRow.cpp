#pragma once
#include <vector>
#include <string>

class CSVRow {
private:
    std::vector<std::string> values;
    std::vector<std::string> headers;

public:
    CSVRow(const std::vector<std::string>& headers);
    void addValue(const std::string& value);
    std::string getValueByColumnName(const std::string& columnName) const;
    // ... other method declarations ...
};