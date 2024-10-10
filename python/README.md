# CSV Query Engine

## 1. Overview

The CSV Query Engine is a custom-built tool designed to process and query large CSV files efficiently without relying on external libraries like pandas. This tool provides SQL-like querying capabilities for CSV data, focusing on multi-processing techniques to handle large datasets effectively.

Key features:
- SQL-like query syntax for CSV files
- Support for basic SELECT operations with WHERE clauses
- Ability to perform calculations in SELECT statements
- ORDER BY and LIMIT clause support
- Efficient handling of large CSV files through chunking and parallel processing
- Custom implementation without relying on pandas or other data processing libraries

## 2. System Architecture

The CSV Query Engine consists of several key components:
1. DataLoader: Responsible for reading and parsing the CSV file using memory-mapped I/O and parallel processing.
2. QueryEngine: The core component that processes queries and executes them on the loaded data.
3. CLI: Provides a command-line interface for users to interact with the query engine.
4. Main script: Orchestrates the overall flow of the application and handles error management.

## 3. Multi-processing Implementation

The DataLoader now utilizes multi-processing to efficiently handle large CSV files:
- The file is split into chunks, which are processed in parallel.
- The number of processes is set to the number of CPU cores available.
- A memory-mapped file approach is used for efficient file reading.

## 4. File Handling and Query Parsing

### File Handling
- Uses memory-mapped file reading for efficient I/O operations.
- Implements chunking to process large files in manageable pieces.
- Utilizes parallel processing to speed up data loading.

### Query Parsing
(No changes to this section)

## 5. Code Structure and Flow

The main changes are in the DataLoader class and error handling:
1. `main.py`: Now includes comprehensive error handling and user interruption management.
2. `data_loader.py`: Significantly optimized with parallel processing and memory-mapped file reading.
3. `query_engine.py` and `cli.py`: (No significant changes)

## 6. Performance Optimization Techniques

Several optimization techniques have been implemented:
1. Chunking: The file is split into manageable chunks for parallel processing.
2. Memory-mapped I/O: Efficient file reading, especially for large files.
3. Multi-processing: Utilizes all available CPU cores for faster data loading.
4. Dynamic chunk sizing: Adapts chunk size based on file size and available resources.

## 7. Known Issues and Future Improvements

Known Issues:
1. Memory usage: While improved, very large files may still cause memory issues.
2. Limited SQL support: Only basic SELECT operations are supported.

Future Improvements:
1. Implement more complex SQL operations (e.g., JOIN, GROUP BY, aggregate functions).
2. Develop a more robust parsing system for complex queries.
3. Add data streaming capabilities for processing extremely large CSV files.
4. Implement a caching system for query results and intermediate calculations.
5. Add support for multiple file formats beyond CSV.
6. Develop a graphical user interface for easier interaction.

By addressing these improvements, the CSV Query Engine can become an even more robust and efficient tool for processing large datasets without relying on external libraries.