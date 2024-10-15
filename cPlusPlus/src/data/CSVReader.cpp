#include "CSVReader.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>

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

bool CSVReader::isInteger(const std::string& s) const {
    std::istringstream iss(s);
    int n;
    iss >> std::noskipws >> n;
    return iss.eof() && !iss.fail();
}

bool CSVReader::isFloat(const std::string& s) const {
    std::istringstream iss(s);
    float f;
    iss >> std::noskipws >> f;
    return iss.eof() && !iss.fail();
}

ColumnStore::ColumnData CSVReader::determineColumnType(const std::vector<std::string>& values) const {
    bool allInteger = true;
    bool allFloat = true;

    for (const auto& value : values) {
        if (value.empty()) continue;
        if (!isInteger(value)) allInteger = false;
        if (!isFloat(value)) allFloat = false;
        if (!allInteger && !allFloat) break;
    }

    if (allInteger) {
        std::vector<int> intValues;
        for (const auto& value : values) {
            if (value.empty()) {
                intValues.push_back(0);
            } else {
                std::istringstream iss(value);
                int n;
                iss >> n;
                intValues.push_back(n);
            }
        }
        return intValues;
    } else if (allFloat) {
        std::vector<float> floatValues;
        for (const auto& value : values) {
            if (value.empty()) {
                floatValues.push_back(0.0f);
            } else {
                std::istringstream iss(value);
                float f;
                iss >> f;
                floatValues.push_back(f);
            }
        }
        return floatValues;
    } else {
        return values;
    }
}

ColumnStore CSVReader::readCSV() {
    ColumnStore data;
    std::ifstream file(filename);
    std::string line;

    std::getline(file, line);
    std::vector<std::string> headers = splitLine(line);


    std::unordered_map<std::string, std::vector<std::string>> tempColumns;
    for (const auto& header : headers) {
        tempColumns[header] = std::vector<std::string>();
    }

    while (std::getline(file, line)) {
        std::vector<std::string> values = splitLine(line);
        for (size_t i = 0; i < headers.size() && i < values.size(); ++i) {
            tempColumns[headers[i]].push_back(values[i]);
        }
    }

    for (const auto& [header, values] : tempColumns) {
        data.addColumn(header, determineColumnType(values));
    }

    return data;
}
