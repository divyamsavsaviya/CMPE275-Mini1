## 1. Overview

The CSV Query Engine is a custom-built tool designed to process and query large CSV files without relying on external libraries like pandas. This tool aims to provide SQL-like querying capabilities for CSV data while focusing on multi-processing and multi-threading techniques in a distributed computing context. The engine allows users to perform SELECT operations with conditions, calculations, ordering, and limiting results.

Key features:

- SQL-like query syntax for CSV files
- Support for basic SELECT operations with WHERE clauses
- Ability to perform calculations in SELECT statements
- ORDER BY and LIMIT clause support
- Custom implementation without relying on pandas or other data processing libraries

## 2. System Architecture

The CSV Query Engine is composed of several key components:

1. DataLoader: Responsible for reading and parsing the CSV file.
2. QueryEngine: The core component that processes queries and executes them on the loaded data.
3. CLI: Provides a command-line interface for users to interact with the query engine.
4. Main script: Orchestrates the overall flow of the application.

The system follows a modular design, with each component handling specific responsibilities:

```
[CSV File] -> DataLoader -> [Raw Data] -> QueryEngine <- [User Query] <- CLI
                                 |
                                 v
                            [Query Results] -> CLI -> [User Output]

```

## 3. Multi-processing and Multi-threading Implementation

While the current implementation doesn't explicitly use multi-processing or multi-threading, the architecture is designed to accommodate these features in future iterations. Potential areas for parallelization include:

- Data loading: Splitting the CSV file into chunks and loading them in parallel.
- Query execution: Processing different parts of the data concurrently, especially for large datasets.
- Calculation operations: Performing complex calculations across multiple threads.

To implement these features, you could use Python's `multiprocessing` and `threading` modules. For example:

```python
from multiprocessing import Pool

def process_chunk(chunk):
    # Process a chunk of data
    pass

with Pool(processes=4) as pool:
    results = pool.map(process_chunk, data_chunks)

```

## 4. File Handling and Query Parsing

### File Handling

The `DataLoader` class handles CSV file reading:

- Uses Python's built-in `csv` module to read the file.
- Stores data in memory as a list of lists.
- Extracts column names from the first row of the CSV.

### Query Parsing

The `CLI` class parses user queries using regular expressions:

- Splits the query into SELECT, WHERE, ORDER BY, and LIMIT clauses.
- Handles quoted column names and string values.
- Supports basic arithmetic operations in SELECT statements.

Query syntax example:

```sql
SELECT "Country Name", 1960, 2020, (2020-1960)/1960*100 AS "Growth Rate"
WHERE "Country Name"="Japan" OR "Country Name"="Germany"
ORDER BY "Growth Rate" DESC
LIMIT 10

```

## 5. Code Structure and Flow

1. `main.py`: Entry point of the application
    - Parses command-line arguments
    - Initializes DataLoader, QueryEngine, and CLI
    - Runs the main application loop
2. `data_loader.py`: Contains the DataLoader class
    - `load()`: Reads the CSV file and returns data and columns
3. `query_engine.py`: Contains the QueryEngine class
    - `execute_query()`: Main method for executing queries
    - `evaluate_conditions()`: Evaluates WHERE clause conditions
    - `perform_calculation()`: Handles calculations in SELECT statements
4. `cli.py`: Contains the CLI class
    - `run()`: Main loop for user interaction
    - `parse_query()`: Parses user input into query components
    - `display_results()`: Formats and displays query results

Flow:

1. User provides CSV file path
2. DataLoader reads the CSV file
3. QueryEngine is initialized with the loaded data
4. CLI prompts user for queries
5. Each query is parsed and executed by the QueryEngine
6. Results are displayed to the user
7. Process repeats until the user exits

## 6. Performance Optimization Techniques

While the current implementation focuses on functionality rather than optimization, several techniques could be employed to improve performance:

1. Indexing: Implement an indexing system for frequently queried columns to speed up lookups.
2. Lazy evaluation: Process data only when necessary, especially for large datasets.
3. Caching: Store results of common subqueries or calculations to avoid redundant processing.
4. Data streaming: Instead of loading the entire CSV into memory, implement a streaming approach for very large files.

Example of a simple indexing implementation:

```python
class Index:
    def __init__(self, column):
        self.column = column
        self.index = defaultdict(list)

    def build(self, data, columns):
        col_index = columns.index(self.column)
        for row_index, row in enumerate(data):
            value = row[col_index]
            self.index[value].append(row_index)

    def get_rows(self, value):
        return self.index.get(value, [])

```

## 7. Known Issues and Future Improvements

Known Issues:

1. Memory usage: Loading entire CSV into memory limits the size of files that can be processed.
2. Limited SQL support: Only basic SELECT operations are supported.
3. Performance: Large datasets may process slowly due to lack of optimization.

Future Improvements:

1. Implement multi-processing for data loading and query execution.
2. Add support for more complex SQL operations (e.g., JOIN, GROUP BY, aggregate functions).
3. Implement a more robust parsing system for complex queries.
4. Add data streaming capabilities for processing very large CSV files.
5. Implement a caching system for query results and intermediate calculations.
6. Add support for multiple file formats beyond CSV.
7. Develop a graphical user interface for easier interaction.

To implement these improvements, consider:

- Using the `multiprocessing` module for parallel data processing.
- Implementing a more sophisticated query parser, possibly using a parsing library like `pyparsing`.
- Utilizing generator functions for data streaming.
- Implementing a query plan optimizer to improve execution speed for complex queries.

By addressing these improvements, the CSV Query Engine can become a more robust and efficient tool for processing large datasets without relying on external libraries.