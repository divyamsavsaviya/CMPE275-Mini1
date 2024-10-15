#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "ColumnStore.h"

class CSVReader {
private:
    std::string filename;
    std::string removeQuotes(const std::string& str) const;
    std::vector<std::string> splitLine(const std::string& line) const;
    ColumnStore::ColumnData determineColumnType(const std::vector<std::string>& values) const;
    bool isNumeric(const std::string& s) const;

public:
    CSVReader(const std::string& filename);
    ColumnStore readCSV();
};
