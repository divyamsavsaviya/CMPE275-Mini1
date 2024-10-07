#include "DataApi.h"

DataApi::DataApi(const std::string& filename) : csvReaderFacade(filename) {}

std::vector<std::unordered_map<std::string, std::string>> DataApi::getAllData() {
    return csvReaderFacade.getAllData();
}

std::unordered_map<std::string, std::string>* DataApi::getByCountryCode(const std::string& countryCode) {
    return csvReaderFacade.getByCountryCode(countryCode);
}
