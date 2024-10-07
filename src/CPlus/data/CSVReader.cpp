#pragma once
#include "CSVRow.cpp"
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>

class CSVReader {
private:
    std::string filename;

public:
    CSVReader(const std::string& file) : filename(file) {
//        std::cout << "Inside CSVReader constructor!" << std::endl;
}

    std::vector<CSVRow> readCSV() {
//        std::cout << "Inside CSVReader readCsv method!" << std::endl;

        std::vector<CSVRow> data;
//        std::cout << "Inside CSVReader BP1" << std::endl;

        std::ifstream fileStream(filename);
//        std::cout << "Inside CSVReader BP2" << std::endl;

        std::string line, word;
//        std::cout << "Inside CSVReader BP3" << line << word << std::endl;

        // Skip the header line
        std::getline(fileStream, line);
//        std::cout << "Inside CSVReader BP4" << std::endl;

        // Read each line from CSV
        while (std::getline(fileStream, line)) {
//            std::cout << "Inside CSVReader BP5" << std::endl;

            std::stringstream ss(line);
            std::vector<std::string> row;

            while (std::getline(ss, word, ',')) {
                row.push_back(word);
            }

            // Assuming the order: Country_Name, Country_Code, Indicator_Name, Indicator_Code, Year_1960, Year_1961...
            if (row.size() >= 6) {
//                    std::cout << "inside csvreader printing! rowSize: " << row.size() << std::endl;

                CSVRow csvRow(row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8], row[9], row[10],
                row[11], row[12], row[13], row[14], row[15], row[16], row[17], row[18], row[19], row[20], row[21],
                row[22], row[23], row[24], row[25], row[26], row[27], row[28], row[29], row[30], row[31], row[32],
                row[33], row[34], row[35], row[36], row[37], row[38], row[39], row[40], row[41], row[42], row[43],
                row[44], row[45], row[46], row[47], row[48], row[49], row[50], row[51], row[52], row[53], row[54],
                row[55], row[56], row[57], row[58], row[59], row[60], row[61], row[62], row[63], row[64], row[65],
                row[66], row[67]);
                data.push_back(csvRow);
            }
        }

        return data;
    }
};
