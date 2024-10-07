#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include "QueryEngine.h"

class CLI {
private:
    QueryEngine& queryEngine;

    // Add this trim function
    static std::string trim(const std::string& str) {
        auto start = str.begin();
        while (start != str.end() && std::isspace(*start)) {
            start++;
        }
        auto end = str.end();
        do {
            end--;
        } while (std::distance(start, end) > 0 && std::isspace(*end));
        return std::string(start, end + 1);
    }

    std::tuple<std::string, std::string, std::string, std::string> parseQuery(const std::string& query) {
        std::regex selectPattern(R"(SELECT\s+(.*?)\s*(?:WHERE|ORDER BY|LIMIT|$))", std::regex::icase);
        std::regex wherePattern(R"(WHERE\s+(.*?)\s*(?:ORDER BY|LIMIT|$))", std::regex::icase);
        std::regex orderByPattern(R"(ORDER BY\s+(.*?)\s*(?:LIMIT|$))", std::regex::icase);
        std::regex limitPattern(R"(LIMIT\s+(\d+))", std::regex::icase);

        std::smatch match;
        std::string selectPart, wherePart, orderBy, limit;

        if (std::regex_search(query, match, selectPattern)) selectPart = match[1];
        if (std::regex_search(query, match, wherePattern)) wherePart = match[1];
        if (std::regex_search(query, match, orderByPattern)) orderBy = match[1];
        if (std::regex_search(query, match, limitPattern)) limit = match[1];

        return {selectPart, wherePart, orderBy, limit};
    }

    void displayResults(const std::vector<std::unordered_map<std::string, std::string>>& results, const std::vector<std::string>& selectColumns) {
        if (results.empty()) {
            std::cout << "No results found." << std::endl;
            return;
        }

        std::vector<std::string> displayColumns;

         if(selectColumns[0] == "*"){
            for (const auto& pair : results[0]) {
                 displayColumns.push_back(pair.first);
            }
         } else{
            displayColumns = selectColumns;
         }

        // Print header
        std::cout << "Row | ";
        for (const auto& col : displayColumns) {
            std::cout << col << " | ";
        }
        std::cout << std::endl << std::string(80, '-') << std::endl;

        // Print rows
        for (size_t i = 0; i < results.size(); ++i) {
            std::cout << i << " | ";
            for (const auto& col : displayColumns) {
                std::cout << results[i].at(col) << " | ";
            }
            std::cout << std::endl;
        }

        std::cout << "\nTotal results: " << results.size() << std::endl;
    }

public:
    CLI(QueryEngine& qe) : queryEngine(qe) {}

    void run() {
        while (true) {
            std::cout << "\nEnter your query (or 'exit' to quit):" << std::endl;
            std::cout << "Format: SELECT column1,column2,... WHERE condition1 AND condition2 ... ORDER BY column [ASC|DESC] LIMIT n" << std::endl;
            std::string query;
            std::getline(std::cin, query);

            if (query == "exit") break;

            try {
                auto [selectPart, wherePart, orderBy, limit] = parseQuery(query);

                std::vector<std::string> selectColumns;
                std::istringstream selectStream(selectPart);
                std::string column;
                bool inQuotes = false;
                std::string currentColumn;
                char ch;
                while (selectStream.get(ch)) {
                    if (ch == '"') {
                        inQuotes = !inQuotes;
                    } else if (ch == ',' && !inQuotes) {
                        selectColumns.push_back(trim(currentColumn));
                        currentColumn.clear();
                    } else {
                        currentColumn += ch;
                    }
                }
                if (!currentColumn.empty()) {
                    selectColumns.push_back(trim(currentColumn));
                }

                std::vector<std::string> conditions;
                std::istringstream whereStream(wherePart);
                std::string condition;
                while (std::getline(whereStream, condition, ' ')) {
                    if (condition == "AND") continue;
                    conditions.push_back(trim(condition));
                }

                auto results = queryEngine.executeQuery(selectColumns, conditions, orderBy, limit);
                displayResults(results, selectColumns);
            } catch (const std::exception& e) {
                std::cerr << "An error occurred: " << e.what() << std::endl;
            }
        }
    }
};