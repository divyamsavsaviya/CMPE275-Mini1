#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "CSVReader.h"
#include "ColumnStore.h"

class CSVReaderFacade {
private:
    CSVReader reader;
    ColumnStore columnStore;

public:
    CSVReaderFacade(const std::string& filename) : reader(filename) {}

    ColumnStore getAllData();
    std::unordered_map<std::string, std::string> getByCountryCode(const std::string& countryCode);
};
