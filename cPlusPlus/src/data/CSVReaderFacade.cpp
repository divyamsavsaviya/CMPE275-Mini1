#pragma once
#include "CSVReader.cpp"
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>

class CSVReaderFacade {
private:
    CSVReader reader;
    std::vector<CSVRow> data;
    std::unordered_map<std::string, CSVRow*> indexedData;

public:
    CSVReaderFacade(const std::string& filename) : reader(filename) {
//        std::cout << "Inside CSVReaderFacade BP1: " << filename << std::endl;

        data = reader.readCSV();
//        std::cout << "Inside CSVReaderFacade BP2: " << data.size() << std::endl;

        for (auto& row : data) {
            indexedData[row.Country_Code] = &row;
        }
    }

    std::vector<CSVRow> getAllData() {
        return data;
    }

    CSVRow* getByCountryCode(const std::string& countryCode) {
        auto it = indexedData.find(countryCode);
        if (it != indexedData.end()) {
            return it->second;
        }
        return nullptr;
    }
};
