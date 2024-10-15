#include <iostream>
#include <mpi.h>
#include "api/QueryEngine.h"
#include "api/CLI.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <cstring>

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

int main(int argc, char* argv[]) {
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

    std::string filename = argv[1];
    QueryEngine queryEngine(filename);

    if (rank == 0) {
        // Only the root process handles user input
        CLI cli(queryEngine);
        cli.run();
    } else {
        // Other processes wait for work (implement this later)
        // For now, they'll just wait
    }

    MPI_Finalize();
    return 0;
}
