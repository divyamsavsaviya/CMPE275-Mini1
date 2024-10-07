#include "QueryEngine.h"
#include "CSVReaderFacade.h"
#include <algorithm>
#include <stdexcept>
#include <iostream> 

QueryEngine::QueryEngine(const std::string& filename, int argc, char** argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        CSVReaderFacade reader(filename);
        data = reader.getAllData();
        std::cout << "Loaded " << data.size() << " rows of data." << std::endl;
    }

    int dataSize = data.size();
    MPI_Bcast(&dataSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0) {
        data.resize(dataSize);
    }

    MPI_Bcast(data.data(), dataSize * sizeof(std::unordered_map<std::string, std::string>), MPI_BYTE, 0, MPI_COMM_WORLD);
}

QueryEngine::~QueryEngine() {
    MPI_Finalize();
}

std::pair<std::vector<std::unordered_map<std::string, std::string>>, PerformanceMeasurement> 
QueryEngine::executeQueryWithPerformance(const std::vector<std::string>& selectColumns,
                                         const std::vector<std::pair<std::string, std::string>>& conditions,
                                         const std::string& orderBy,
                                         int limit) {
    PerformanceMeasurement perf;
    perf.start();

    auto results = executeQueryParallel(selectColumns, conditions, orderBy, limit);

    perf.stop();
    return {results, perf};
}

std::vector<std::unordered_map<std::string, std::string>> QueryEngine::executeQueryParallel(
    const std::vector<std::string>& selectColumns,
    const std::vector<std::pair<std::string, std::string>>& conditions,
    const std::string& orderBy,
    int limit) {

    int chunkSize = data.size() / size;
    int startIdx = rank * chunkSize;
    int endIdx = (rank == size - 1) ? data.size() : startIdx + chunkSize;

    std::vector<std::unordered_map<std::string, std::string>> localChunk(data.begin() + startIdx, data.begin() + endIdx);
    std::vector<std::unordered_map<std::string, std::string>> localResults = processChunk(localChunk, selectColumns, conditions);

    std::vector<std::unordered_map<std::string, std::string>> globalResults;

    if (rank == 0) {
        globalResults = localResults;

        for (int i = 1; i < size; ++i) {
            int resultSize;
            MPI_Recv(&resultSize, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            std::vector<std::unordered_map<std::string, std::string>> receivedResults(resultSize);
            MPI_Recv(receivedResults.data(), resultSize * sizeof(std::unordered_map<std::string, std::string>), MPI_BYTE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            globalResults.insert(globalResults.end(), receivedResults.begin(), receivedResults.end());
        }

        if (!orderBy.empty()) {
            std::sort(globalResults.begin(), globalResults.end(),
                [&orderBy](const auto& a, const auto& b) {
                    auto it_a = a.find(orderBy);
                    auto it_b = b.find(orderBy);
                    if (it_a == a.end() || it_b == b.end()) {
                        return false;
                    }
                    return it_a->second < it_b->second;
                });
        }

        if (limit > 0 && limit < static_cast<int>(globalResults.size())) {
            globalResults.resize(limit);
        }
    } else {
        int resultSize = localResults.size();
        MPI_Send(&resultSize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(localResults.data(), resultSize * sizeof(std::unordered_map<std::string, std::string>), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
    }

    return globalResults;
}

std::vector<std::unordered_map<std::string, std::string>> 
QueryEngine::processChunk(const std::vector<std::unordered_map<std::string, std::string>>& chunk,
                          const std::vector<std::string>& selectColumns,
                          const std::vector<std::pair<std::string, std::string>>& conditions) {
    std::vector<std::unordered_map<std::string, std::string>> results;

    for (const auto& row : chunk) {
        bool satisfiesConditions = true;
        for (const auto& [column, value] : conditions) {
            std::string quotedColumn = '"' + column + '"';
            auto it = row.find(quotedColumn);
            if (it == row.end() || it->second != value) {
                satisfiesConditions = false;
                break;
            }
        }

        if (satisfiesConditions) {
            std::unordered_map<std::string, std::string> resultRow;
            for (const auto& col : selectColumns) {
                std::string quotedCol = '"' + col + '"';
                auto it = row.find(quotedCol);
                if (it != row.end()) {
                    resultRow[col] = it->second;
                }
            }
            if (!resultRow.empty()) {
                results.push_back(resultRow);
            }
        }
    }

    return results;
}