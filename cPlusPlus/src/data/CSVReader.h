#pragma once
#include <vector>
#include <string>
#include <unordered_map>

class CSVReader {
private:
    std::string filename;
    std::string removeQuotes(const std::string& str) const;
    std::vector<std::string> splitLine(const std::string& line) const;

public:
    CSVReader(const std::string& filename);
    std::vector<std::unordered_map<std::string, std::string>> readCSV();
};
