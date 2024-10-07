#include "CSVReaderFacade.h"
#include "CSVReader.h"

CSVReaderFacade::CSVReaderFacade(const std::string& filename) : filename(filename) {}

std::vector<std::unordered_map<std::string, std::string>> CSVReaderFacade::getAllData() {
    CSVReader reader(filename);
    return reader.readCSV();
}

std::unordered_map<std::string, std::string> CSVReaderFacade::getByCountryCode(const std::string& countryCode) {
    CSVReader reader(filename);
    auto allData = reader.readCSV();
    for (const auto& row : allData) {
        auto it = row.find("Country Code");
        if (it != row.end() && it->second == countryCode) {
            return row;
        }
    }
    return {};  
}
