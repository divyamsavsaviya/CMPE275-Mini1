#include "CSVReader.h"
#include <fstream>
#include <sstream>
#include <algorithm>

CSVReader::CSVReader(const std::string& filename) : filename(filename) {}

std::vector<std::unordered_map<std::string, std::string>> CSVReader::readCSV() {
    std::vector<std::unordered_map<std::string, std::string>> data;
    std::ifstream file(filename);
    std::string line;
    std::vector<std::string> headers;

    if (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string header;
        while (std::getline(iss, header, ',')) {
            headers.push_back(header);
        }
    }

    while (std::getline(file, line)) {
        std::unordered_map<std::string, std::string> row;
        std::istringstream iss(line);
        std::string value;
        size_t i = 0;
        while (std::getline(iss, value, ',')) {
            if (i < headers.size()) {
                row[headers[i]] = value;
            }
            ++i;
        }
        data.push_back(row);
    }

    return data;
}

std::vector<std::string> CSVReader::splitLine(const std::string& line) {
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string item;
    while (std::getline(ss, item, ',')) {
        // Remove leading and trailing whitespace
        item.erase(0, item.find_first_not_of(" \t"));
        item.erase(item.find_last_not_of(" \t") + 1);
        result.push_back(item);
    }
    return result;
}
