#include "CSVReader.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
#include <cctype>

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

bool CSVReader::isNumeric(const std::string& s) const {
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) { 
        return !std::isdigit(c) && c != '.' && c != '-'; 
    }) == s.end();
}

ColumnStore::ColumnData CSVReader::determineColumnType(const std::vector<std::string>& values) const {
    bool allNumeric = true;
    bool hasDecimal = false;

    for (const auto& value : values) {
        if (!value.empty() && !isNumeric(value)) {
            allNumeric = false;
            break;
        }
        if (value.find('.') != std::string::npos) {
            hasDecimal = true;
        }
    }

    if (allNumeric) {
        if (hasDecimal) {
            std::vector<float> floatValues;
            for (const auto& value : values) {
                try {
                    floatValues.push_back(value.empty() ? 0.0f : std::stof(value));
                } catch (const std::exception&) {
                    floatValues.push_back(0.0f);
                }
            }
            return floatValues;
        } else {
            std::vector<int> intValues;
            for (const auto& value : values) {
                try {
                    intValues.push_back(value.empty() ? 0 : std::stoi(value));
                } catch (const std::exception&) {
                    intValues.push_back(0);
                }
            }
            return intValues;
        }
    } else {
        return values; // Keep as strings if not all values are numeric
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
