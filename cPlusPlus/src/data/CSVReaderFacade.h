#pragma once
#include <string>
#include <vector>
#include <unordered_map>

class CSVReaderFacade {
public:
    CSVReaderFacade(const std::string& filename);
    std::vector<std::unordered_map<std::string, std::string>> getAllData();
    std::unordered_map<std::string, std::string> getByCountryCode(const std::string& countryCode);  // Change return type to non-pointer
private:
    std::string filename;
    std::vector<std::unordered_map<std::string, std::string>> data;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>*> indexedData;
};