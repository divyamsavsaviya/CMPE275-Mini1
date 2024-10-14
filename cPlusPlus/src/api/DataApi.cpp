#include "DataApi.h"
#include <sstream>

DataApi::DataApi(const std::string& filename) : mmFile(filename) {
    parseData();
}

void DataApi::parseData() {
    const char* data = static_cast<const char*>(mmFile.getData());
    size_t size = mmFile.getSize();

    std::istringstream stream(std::string(data, size));
    std::string line;
    std::vector<std::string> headers;

    // Read headers
    if (std::getline(stream, line)) {
        std::istringstream headerStream(line);
        std::string header;
        while (std::getline(headerStream, header, ',')) {
            headers.push_back(header);
        }
    }

    // Read data
    while (std::getline(stream, line)) {
        std::istringstream rowStream(line);
        std::string cell;
        std::unordered_map<std::string, std::string> row;
        size_t columnIndex = 0;
        while (std::getline(rowStream, cell, ',')) {
            if (columnIndex < headers.size()) {
                row[headers[columnIndex]] = cell;
            }
            ++columnIndex;
        }
        parsedData.push_back(row);
    }
}

std::vector<std::unordered_map<std::string, std::string>> DataApi::getAllData() {
    return parsedData;
}

std::unordered_map<std::string, std::string> DataApi::getByCountryCode(const std::string& countryCode) {
    for (const auto& row : parsedData) {
        auto it = row.find("Country Code");
        if (it != row.end() && it->second == countryCode) {
            return row;
        }
    }
    return {};
}