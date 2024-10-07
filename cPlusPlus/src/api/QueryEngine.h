#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "../data/CSVReader.h"

class QueryEngine {
public:
    QueryEngine(const std::string& filename);
    std::vector<std::unordered_map<std::string, std::string>> executeQuery(
        const std::vector<std::string>& selectColumns,
        const std::vector<std::pair<std::string, std::string>>& conditions,
        const std::string& orderBy,
        int limit
    );

private:
    std::vector<std::unordered_map<std::string, std::string>> data;
};