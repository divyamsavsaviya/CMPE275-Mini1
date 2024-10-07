#pragma once
#include "../data/CSVReaderFacade.cpp"
#include <iostream>

class DataApi {
private:
    CSVReaderFacade facade;

public:
    DataApi(const std::string& filename) : facade(filename) {
//        std::cout << "Inside DataApi BP1" << filename << std::endl;
}

    std::vector<CSVRow> getAllData() {
//        std::cout << "Inside DataApi BP2" << std::endl;
        return facade.getAllData();
    }

    CSVRow* getByCountryCode(const std::string& countryCode) {
        return facade.getByCountryCode(countryCode);
    }
};
