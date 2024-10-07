#pragma once
#include <vector>
#include <string>
#include <unordered_map>

class CSVReader {
public:
    CSVReader(const std::string& filename);
    std::vector<std::unordered_map<std::string, std::string>> readCSV();

private:
    std::string filename;
    std::vector<std::string> splitLine(const std::string& line);
};