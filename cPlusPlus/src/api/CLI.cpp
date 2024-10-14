#include "CLI.h"
#include <iostream>
#include <sstream>
#include <regex>
#include <chrono>
#include <iomanip>
#include <thread>

CLI::CLI(QueryEngine& qe) : queryEngine(qe) {}

void CLI::run() {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        std::thread queryProcessor([this]() {
            std::string query;
            while (true) {
                if (queryQueue.pop(query)) {
                    if (query == "exit") break;

                    std::cout << "Starting Performance Measurement" << std::endl;
                    auto start_time = std::chrono::high_resolution_clock::now();

                    auto [selectColumns, conditions, orderBy, limit] = parseQuery(query);
                    auto [results, queryPerf] = queryEngine.executeQueryWithPerformance(selectColumns, conditions, orderBy, limit);

                    auto end_time = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1e6;

                    std::cout << "Stopping Performance Measurement" << std::endl;

                    displayFormattedResults(results, selectColumns);
                    displayPerformanceMetrics(queryPerf, results.size(), duration);
                }
            }
        });

        std::string input;
        while (true) {
            std::cout << "\nEnter your query (or 'exit' to quit):\n";
            std::cout << "Format: SELECT column1,column2,... WHERE condition1 AND condition2 ... ORDER BY column [ASC|DESC] LIMIT n\n> ";
            std::getline(std::cin, input);

            queryQueue.push(input);

            if (input == "exit") {
                break;
            }
        }

        queryProcessor.join();
    }

    MPI_Barrier(MPI_COMM_WORLD);
}

void CLI::displayFormattedResults(const std::vector<std::unordered_map<std::string, std::string>>& results, const std::vector<std::string>& selectColumns) {
    if (results.empty() || selectColumns.empty()) {
        std::cout << "No results found." << std::endl;
        return;
    }

    // Print header
    std::cout << "Row | ";
    for (const auto& col : selectColumns) {
        std::cout << "\"" << col << "\" | ";
    }
    std::cout << "\n" << std::string(80, '-') << std::endl;

    // Print rows
    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << i + 1 << " | ";
        for (const auto& col : selectColumns) {
            auto it = results[i].find(col);
            if (it != results[i].end()) {
                std::cout << it->second << " | ";
            } else {
                std::cout << "N/A | ";
            }
        }
        std::cout << std::endl;
    }
}

void CLI::displayPerformanceMetrics(const PerformanceMeasurement& perf, size_t resultCount, double totalTime) {
    std::cout << "\nPerformance Metrics:" << std::endl;
    std::cout << "Total results: " << resultCount << std::endl;
    std::cout << "CPU Time: " << perf.getCPUTime() << " seconds" << std::endl;
    std::cout << "Memory Used: " << perf.getMemoryUsage() << " bytes" << std::endl;
    std::cout << "Total Execution Time: " << totalTime << " seconds" << std::endl;
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
            selectColumns.push_back(this->trim(column));
        }
    }

    if (std::regex_search(query, match, wherePattern)) {
        std::string whereClause = match[1];
        std::regex conditionPattern(R"(("?[\w\s]+"?)\s*(=|>|<)\s*("?[\w\s]+"?))");
        auto words_begin = std::sregex_iterator(whereClause.begin(), whereClause.end(), conditionPattern);
        auto words_end = std::sregex_iterator();

        for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
            std::smatch match = *i;
            std::string column = this->removeQuotes(this->trim(match[1]));
            std::string op = match[2];
            std::string value = this->removeQuotes(this->trim(match[3]));
            conditions.emplace_back(column, value);
        }
    }

    if (std::regex_search(query, match, orderByPattern)) {
        orderBy = this->trim(match[1]);
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
    std::string result = str;
    result.erase(std::remove(result.begin(), result.end(), '\"'), result.end());
    return result;
}

std::string CLI::trim(const std::string& str) {
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
