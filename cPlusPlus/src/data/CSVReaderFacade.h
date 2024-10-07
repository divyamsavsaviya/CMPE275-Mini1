#pragma once
#include "CSVReader.h"
#include <vector>
#include <unordered_map>
#include <string>

class CSVReaderFacade {
public:
    CSVReaderFacade(const std::string& filename);
    std::vector<std::unordered_map<std::string, std::string>> getAllData();
    std::unordered_map<std::string, std::string>* getByCountryCode(const std::string& countryCode);

private:
    std::vector<std::unordered_map<std::string, std::string>> data;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>*> indexedData;
};