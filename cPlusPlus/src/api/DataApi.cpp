#include "DataApi.h"

DataApi::DataApi(const std::string& filename) : csvReader(filename) {}

std::vector<std::unordered_map<std::string, std::string>> DataApi::getAllData() {
    return csvReader.getAllData();
}

std::unordered_map<std::string, std::string> DataApi::getByCountryCode(const std::string& countryCode) {
    return csvReader.getByCountryCode(countryCode);
}