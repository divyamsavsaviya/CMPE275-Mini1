#include "CSVReaderFacade.h"

CSVReaderFacade::CSVReaderFacade(const std::string& filename) {
    CSVReader reader(filename);
    data = reader.readCSV();

    for (auto& row : data) {
        if (row.find("Country Code") != row.end()) {
            indexedData[row["Country Code"]] = &row;
        }
    }
}

std::vector<std::unordered_map<std::string, std::string>> CSVReaderFacade::getAllData() {
    return data;
}

std::unordered_map<std::string, std::string>* CSVReaderFacade::getByCountryCode(const std::string& countryCode) {
    auto it = indexedData.find(countryCode);
    return it != indexedData.end() ? it->second : nullptr;
}
