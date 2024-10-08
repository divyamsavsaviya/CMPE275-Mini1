# CSV Query Engine Architecture

## Overview

Our CSV Query Engine is designed to efficiently process large CSV files using SQL-like queries. It employs a column-oriented storage system and leverages parallel processing to optimize query performance.

## Key Components

1. Data Loader (data_loader.py):
   - Responsible for reading CSV files and populating the ColumnStore.

2. Column Store (column_store.py):
   - Implements a column-oriented data structure for efficient data storage and retrieval.

3. Query Engine (query_engine.py):
   - Processes SQL-like queries using parallel processing techniques.
   - Implements filtering, sorting, and result compilation.

4. CLI (cli.py):
   - Provides a command-line interface for users to input queries and view results.

## Why This Architecture?

1. Column-Oriented Storage:
   - Optimizes memory usage and improves performance for analytical queries that typically access a subset of columns.

2. Parallel Processing:
   - Utilizes multi-threading for I/O-bound operations (filtering, column selection).
   - Employs multi-processing for CPU-bound operations (sorting).

3. Caching Mechanism:
   - Implements an LRU cache to optimize performance for repeated queries.

## How It Aligns with Best Practices

1. Separation of Concerns:
   - Each component has a distinct responsibility, enhancing modularity and maintainability.

2. Efficient Data Structures:
   - The ColumnStore is optimized for the types of operations common in analytical queries.

3. Scalability:
   - Parallel processing allows the engine to scale with available computational resources.

4. Extensibility:
   - The modular design allows for easy addition of new features or optimizations.

For detailed performance metrics and benchmarks, please refer to `PERFORMANCE.md`.