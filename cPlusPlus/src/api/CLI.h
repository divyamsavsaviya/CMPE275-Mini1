#pragma once
#include "QueryEngine.h"
#include <string>
#include <vector>
#include <utility>

class CLI {
public:
    CLI(QueryEngine& qe);
    void run();

private:
    QueryEngine& queryEngine;
    std::tuple<std::vector<std::string>, std::vector<std::pair<std::string, std::string>>, std::string, int> parseQuery(const std::string& query);
    void displayResults(const std::vector<std::unordered_map<std::string, std::string>>& results);
    static std::string trim(const std::string& str);
    std::string removeQuotes(const std::string& str);
};