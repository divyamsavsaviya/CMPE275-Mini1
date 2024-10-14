#include <iostream>
#include <mpi.h> 
#include "QueryEngine.h"
#include "CLI.h"

int main(int argc, char** argv) {
    std::cout << "inside main function: " << std::endl;
    std::cout.flush();
    // MPI_Init(&argc, &argv); 
    std::cout << "after MPI_Init" << std::endl;
    std::cout.flush();

    if (argc != 2) {
        std::cout << "inside if condition: " << std::endl;
        std::cout.flush();

        std::cerr << "Usage: " << argv[0] << " <csv_file_path>" << std::endl;
        std::cout.flush();
        MPI_Finalize();  
        return 1;
    }
    std::string filename = argv[1];
    std::cout << "filename: " << filename << std::endl;
    std::cout.flush();

    QueryEngine queryEngine(filename, argc, argv);
    CLI cli(queryEngine);
    cli.run();

    // MPI_Finalize(); 
    return 0;
}
