#pragma once
#include "../data/CSVReaderFacade.h"
#include <string>
#include <vector>
#include <unordered_map>

class DataApi {
public:
    DataApi(const std::string& filename);
    std::vector<std::unordered_map<std::string, std::string>> getAllData();
    std::unordered_map<std::string, std::string>* getByCountryCode(const std::string& countryCode);

private:
    CSVReaderFacade csvReaderFacade;
};