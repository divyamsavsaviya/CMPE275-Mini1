#pragma once
#include "CSVRow.h"
#include <fstream>
#include <vector>
#include <sstream>

class CSVReader {
private:
    std::string filename;

public:
    CSVReader(const std::string& file) : filename(file) {}

    std::vector<CSVRow> readCSV() {
        std::vector<CSVRow> data;
        std::ifstream fileStream(filename);
        std::string line, word;

        // Skip the header line
        std::getline(fileStream, line);

        // Read each line from CSV
        while (std::getline(fileStream, line)) {
            std::stringstream ss(line);
            std::vector<std::string> row;

            while (std::getline(ss, word, ',')) {
                row.push_back(word);
            }

            // Assuming the order: Country_Name, Country_Code, Indicator_Name, Indicator_Code, Year_1960, Year_1961...
            if (row.size() >= 6) {
                CSVRow csvRow(row[0], row[1], row[2], row[3], row[4], row[5]);
                data.push_back(csvRow);
            }
        }

        return data;
    }
};
