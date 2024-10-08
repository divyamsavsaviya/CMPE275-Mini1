import os
import sys
import argparse
import time
import psutil

# Add the project root directory to the Python path
project_root = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, project_root)

from src import DataLoader, QueryEngine, CLI

def main():
    parser = argparse.ArgumentParser(description='CSV Query Engine')
    parser.add_argument('file_path', type=str, help='Path to the CSV file')
    args = parser.parse_args()

    print(f"Debug: Attempting to load file: {args.file_path}")  # Debug print
    start_time = time.time()
    data_loader = DataLoader(args.file_path)
    data, columns = data_loader.load_data()
    print(f"Debug: Loaded data length: {len(data)}")
    print(f"Debug: Loaded columns: {columns}")
    load_time = time.time() - start_time
    print(f"Data loading time: {load_time:.2f} seconds")

    if data is None or columns is None:
        print("Error: Failed to load data")
        return

    print(f"Columns: {columns}")
    print(f"Number of rows: {len(data)}")

    query_engine = QueryEngine(data, columns)
    cli = CLI(query_engine)
    cli.run()

if __name__ == '__main__':
    main()