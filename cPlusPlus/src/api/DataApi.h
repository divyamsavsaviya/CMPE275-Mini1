#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "../data/CSVReaderFacade.h"

class DataApi {
public:
    DataApi(const std::string& filename);
    std::vector<std::unordered_map<std::string, std::string>> getAllData();
    std::unordered_map<std::string, std::string> getByCountryCode(const std::string& countryCode);  // Remove the pointer
private:
    CSVReaderFacade csvReader;
};