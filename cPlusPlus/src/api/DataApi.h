#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "../data/MemoryMappedFile.h"

class DataApi {
public:
    DataApi(const std::string& filename);
    std::vector<std::unordered_map<std::string, std::string>> getAllData();
    std::unordered_map<std::string, std::string> getByCountryCode(const std::string& countryCode);

private:
    MemoryMappedFile mmFile;
    void parseData();
    std::vector<std::unordered_map<std::string, std::string>> parsedData;
};