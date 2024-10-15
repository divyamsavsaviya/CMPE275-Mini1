#pragma once
#include "QueryEngine.h"
#include "PerformanceMeasurement.h"
#include "../data/LockFreeQueue.h"
#include <mpi.h> 
#include <string>
#include <vector>
#include <utility>
#include <tuple>
#include <unordered_map>

class CLI {
public:
    CLI(QueryEngine& qe);
    void run();
    std::tuple<std::vector<std::string>, std::vector<std::pair<std::string, std::string>>, std::string, int> parseQuery(const std::string& query);
    void displayFormattedResults(const std::vector<std::unordered_map<std::string, std::string>>& results, const std::vector<std::string>& selectColumns);
    

private:
    QueryEngine& queryEngine;
    LockFreeQueue<std::string> queryQueue;
    void displayPerformanceMetrics(const PerformanceMeasurement& perf, size_t resultCount, double totalTime);
    
    std::string removeQuotes(const std::string& str);
    std::string trim(const std::string& str);
};