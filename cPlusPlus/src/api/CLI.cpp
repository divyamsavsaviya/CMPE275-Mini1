#include "CLI.h"
#include <iostream>
#include <sstream>
#include <regex>

CLI::CLI(QueryEngine& qe) : queryEngine(qe) {}

void CLI::run() {
    while (true) {
        std::cout << "\nEnter your query (or 'exit' to quit):" << std::endl;
        std::string query;
        std::getline(std::cin, query);

        if (query == "exit") break;

        try {
            auto [selectColumns, conditions, orderBy, limit] = parseQuery(query);
            auto results = queryEngine.executeQuery(selectColumns, conditions, orderBy, limit);
            displayResults(results);
        } catch (const std::exception& e) {
            std::cerr << "An error occurred: " << e.what() << std::endl;
        }
    }
}

std::tuple<std::vector<std::string>, std::vector<std::pair<std::string, std::string>>, std::string, int> CLI::parseQuery(const std::string& query) {
    std::vector<std::string> selectColumns;
    std::vector<std::pair<std::string, std::string>> conditions;
    std::string orderBy;
    int limit = -1;

    std::regex selectPattern(R"(SELECT\s+(.*?)\s+(?:WHERE|ORDER BY|LIMIT|$))");
    std::regex wherePattern(R"(WHERE\s+(.*?)(?:\s+ORDER BY|\s+LIMIT|$))");
    std::regex orderByPattern(R"(ORDER BY\s+(.*?)(?:\s+LIMIT|$))");
    std::regex limitPattern(R"(LIMIT\s+(\d+))");

    std::smatch match;

    if (std::regex_search(query, match, selectPattern)) {
        std::istringstream iss(match[1]);
        std::string column;
        while (std::getline(iss, column, ',')) {
            selectColumns.push_back(trim(column));
        }
    }

    if (std::regex_search(query, match, wherePattern)) {
        std::string whereClause = match[1];
        std::regex conditionPattern(R"(("?[\w\s]+"?)\s*(=|>|<)\s*("?[\w\s]+"?))");
        auto words_begin = std::sregex_iterator(whereClause.begin(), whereClause.end(), conditionPattern);
        auto words_end = std::sregex_iterator();

        for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
            std::smatch match = *i;
            std::string column = removeQuotes(trim(match[1]));
            std::string op = match[2];
            std::string value = removeQuotes(trim(match[3]));
            conditions.emplace_back(column, value);
        }
    }

    if (std::regex_search(query, match, orderByPattern)) {
        orderBy = trim(match[1]);
    }

    if (std::regex_search(query, match, limitPattern)) {
        limit = std::stoi(match[1]);
    }

    std::cout << "Parsed query:" << std::endl;
    std::cout << "Select columns: ";
    for (const auto& col : selectColumns) std::cout << col << ", ";
    std::cout << std::endl;
    
    std::cout << "Conditions: ";
    for (const auto& [col, val] : conditions) std::cout << col << "=" << val << ", ";
    std::cout << std::endl;
    
    std::cout << "Order by: " << orderBy << std::endl;
    std::cout << "Limit: " << limit << std::endl;

    return {selectColumns, conditions, orderBy, limit};
}

std::string CLI::removeQuotes(const std::string& str) {
    if (str.size() >= 2 && str.front() == '"' && str.back() == '"') {
        return str.substr(1, str.size() - 2);
    }
    return str;
}

void CLI::displayResults(const std::vector<std::unordered_map<std::string, std::string>>& results) {
    if (results.empty()) {
        std::cout << "No results found." << std::endl;
        return;
    }

    // Print header
    for (const auto& [key, value] : results[0]) {
        std::cout << key << " | ";
    }
    std::cout << std::endl;

    // Print separator
    for (size_t i = 0; i < results[0].size(); ++i) {
        std::cout << "----------------";
    }
    std::cout << std::endl;

    // Print rows
    for (const auto& row : results) {
        for (const auto& [key, value] : row) {
            std::cout << value << " | ";
        }
        std::cout << std::endl;
    }

    std::cout << "\nTotal results: " << results.size() << std::endl;
}

// Helper function to trim whitespace from a string
std::string CLI::trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}