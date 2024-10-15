#include "CSVReaderFacade.h"
#include "CSVReader.h"
#include <stdexcept>
#include <variant>

ColumnStore CSVReaderFacade::getAllData() {
    if (columnStore.getColumns().empty()) {
        columnStore = reader.readCSV();
    }
    return columnStore;
}

std::unordered_map<std::string, std::string> CSVReaderFacade::getByCountryCode(const std::string& countryCode) {
    if (columnStore.getColumns().empty()) {
        columnStore = reader.readCSV();
    }

    std::unordered_map<std::string, std::string> result;
    const auto& columns = columnStore.getColumns();
    auto countryCodeIt = columns.find("Country Code");
    
    if (countryCodeIt == columns.end()) {
        throw std::runtime_error("Country Code column not found");
    }

    const auto& countryCodeColumn = std::get<std::vector<std::string>>(countryCodeIt->second);
    
    for (size_t i = 0; i < countryCodeColumn.size(); ++i) {
        if (countryCodeColumn[i] == countryCode) {
            for (const auto& [columnName, columnData] : columns) {
                std::visit([&](const auto& data) {
                    using T = std::decay_t<decltype(data[i])>;
                    if constexpr (std::is_same_v<T, std::string>) {
                        result[columnName] = data[i];
                    } else if constexpr (std::is_arithmetic_v<T>) {
                        result[columnName] = std::to_string(data[i]);
                    }
                }, columnData);
            }
            return result;
        }
    }

    return result;
}
