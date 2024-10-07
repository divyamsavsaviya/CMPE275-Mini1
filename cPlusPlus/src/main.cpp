#include "api/PerformanceMeasurement.h"
#include "api/QueryEngine.h"
#include "api/CLI.h"
#include <iostream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <csv_file_path>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    QueryEngine queryEngine(filename, argc, argv);
    CLI cli(queryEngine);
    cli.run();

    return 0;
}
