#include "CSVReader.h"
#include <fstream>
#include <sstream>
#include <algorithm>

CSVReader::CSVReader(const std::string& filename) : filename(filename) {}

std::string CSVReader::removeQuotes(const std::string& str) const {
    std::string result = str;
    result.erase(std::remove(result.begin(), result.end(), '"'), result.end());
    return result;
}

std::vector<std::string> CSVReader::splitLine(const std::string& line) const {
    std::vector<std::string> result;
    std::istringstream iss(line);
    std::string token;
    while (std::getline(iss, token, ',')) {
        result.push_back(removeQuotes(token));
    }
    return result;
}

std::vector<std::unordered_map<std::string, std::string>> CSVReader::readCSV() {
    std::vector<std::unordered_map<std::string, std::string>> data;
    std::ifstream file(filename);
    std::string line;

    // Read header
    std::getline(file, line);
    std::vector<std::string> headers = splitLine(line);

    // Read data
    while (std::getline(file, line)) {
        std::vector<std::string> values = splitLine(line);
        std::unordered_map<std::string, std::string> row;
        for (size_t i = 0; i < headers.size() && i < values.size(); ++i) {
            row[headers[i]] = values[i];
        }
        data.push_back(row);
    }

    return data;
}
