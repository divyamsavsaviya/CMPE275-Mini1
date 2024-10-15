#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "DataApi.h"
#include "../data/ColumnStore.h"
#include "../data/ThreadPool.h"
#include "../data/CSVReader.h"
#include "PerformanceMeasurement.h"
#include <mpi.h>


class QueryEngine {
private:
    ColumnStore dataStore;
    ThreadPool threadPool;
    std::unique_ptr<CSVReader> csvReader;
    int mpi_rank;
    int mpi_size;

    void initializeFromFile(const std::string& filename);

public:
    QueryEngine();
    QueryEngine(const std::string& filename);
    ~QueryEngine() = default;

    std::vector<std::unordered_map<std::string, std::string>> executeQuery(
        const std::vector<std::string>& selectColumns,
        const std::vector<std::pair<std::string, std::string>>& conditions,
        const std::string& orderBy,
        int limit);

    void serializeData(std::vector<char>& buffer) const;
    void deserializeData(const std::vector<char>& buffer);
};
