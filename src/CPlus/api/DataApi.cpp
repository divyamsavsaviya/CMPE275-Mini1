#pragma once
#include "../data/CSVReaderFacade.h"

class DataApi {
private:
    CSVReaderFacade facade;

public:
    DataApi(const std::string& filename) : facade(filename) {}

    std::vector<CSVRow> getAllData() {
        return facade.getAllData();
    }

    CSVRow* getByCountryCode(const std::string& countryCode) {
        return facade.getByCountryCode(countryCode);
    }
};
