#pragma once
#include <string>

class CSVRow {
public:
    std::string Country_Name;
    std::string Country_Code;
    std::string Indicator_Name;
    std::string Indicator_Code;
    std::string Year_1960;
    std::string Year_1961;
    // Add all the necessary years up to Year_2023...

    // Constructor to initialize from CSV data
    CSVRow(const std::string& countryName, const std::string& countryCode,
           const std::string& indicatorName, const std::string& indicatorCode,
           const std::string& year1960, const std::string& year1961)
        : Country_Name(countryName), Country_Code(countryCode),
          Indicator_Name(indicatorName), Indicator_Code(indicatorCode),
          Year_1960(year1960), Year_1961(year1961) {}

    // Method to print row data for testing purposes
    void print() const {
        std::cout << "Country: " << Country_Name << ", Code: " << Country_Code
                  << ", Indicator: " << Indicator_Name << ", Year 1960: " << Year_1960 << std::endl;
    }
};
