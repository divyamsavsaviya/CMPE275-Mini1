#include <iostream>
#include <mpi.h>
#include "api/QueryEngine.h"
#include "api/CLI.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <cstring>

// Helper function to broadcast a vector of strings
void broadcastVector(const std::vector<std::string>& vec, int root) {
    int size = vec.size();
    MPI_Bcast(&size, 1, MPI_INT, root, MPI_COMM_WORLD);
    
    for (const auto& str : vec) {
        int len = str.length();
        MPI_Bcast(&len, 1, MPI_INT, root, MPI_COMM_WORLD);
        char* buffer = new char[len + 1];
        strcpy(buffer, str.c_str());
        MPI_Bcast(buffer, len + 1, MPI_CHAR, root, MPI_COMM_WORLD);
        delete[] buffer;
    }
}

// Helper function to receive a broadcasted vector of strings
std::vector<std::string> receiveBroadcastVector(int root) {
    std::vector<std::string> vec;
    int size;
    MPI_Bcast(&size, 1, MPI_INT, root, MPI_COMM_WORLD);
    vec.reserve(size);
    
    for (int i = 0; i < size; ++i) {
        int len;
        MPI_Bcast(&len, 1, MPI_INT, root, MPI_COMM_WORLD);
        char* buffer = new char[len + 1];
        MPI_Bcast(buffer, len + 1, MPI_CHAR, root, MPI_COMM_WORLD);
        vec.emplace_back(buffer);
        delete[] buffer;
    }
    return vec;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2) {
        if (rank == 0) {
            std::cerr << "Usage: " << argv[0] << " <csv_file_path>" << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    QueryEngine* queryEngine = nullptr;
    if (rank == 0) {
        queryEngine = new QueryEngine(argv[1]);
        // Broadcast the data to other processes
        std::vector<char> buffer;
        queryEngine->serializeData(buffer);
        
        int bufferSize = buffer.size();
        MPI_Bcast(&bufferSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(buffer.data(), bufferSize, MPI_CHAR, 0, MPI_COMM_WORLD);
    } else {
        queryEngine = new QueryEngine(); // Create an empty QueryEngine
        // Receive the broadcast data and populate the QueryEngine
        int bufferSize;
        MPI_Bcast(&bufferSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
        std::vector<char> buffer(bufferSize);
        MPI_Bcast(buffer.data(), bufferSize, MPI_CHAR, 0, MPI_COMM_WORLD);
        queryEngine->deserializeData(buffer);
    }

    CLI cli(*queryEngine);

    if (rank == 0) {
        cli.run();
    } else {
        // Worker processes wait for queries
        while (true) {
            // Receive query from master process
            // Execute query
            // Send results back to master process
        }
    }

    delete queryEngine;
    MPI_Finalize();
    return 0;
}
