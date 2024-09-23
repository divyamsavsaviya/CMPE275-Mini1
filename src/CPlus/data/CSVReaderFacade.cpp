#pragma once
#include "CSVReader.h"
#include <unordered_map>

class CSVReaderFacade {
private:
    CSVReader reader;
    std::vector<CSVRow> data;
    std::unordered_map<std::string, CSVRow> indexedData;

public:
    CSVReaderFacade(const std::string& filename) : reader(filename) {
        data = reader.readCSV();
        for (const auto& row : data) {
            indexedData[row.Country_Code] = row;
        }
    }

    // Return all data
    std::vector<CSVRow> getAllData() {
        return data;
    }

    // Return data by country code
    CSVRow* getByCountryCode(const std::string& countryCode) {
        if (indexedData.find(countryCode) != indexedData.end()) {
            return &indexedData[countryCode];
        }
        return nullptr;
    }
};
