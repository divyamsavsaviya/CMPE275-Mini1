import os
import sys
import argparse
import time
import psutil
import traceback

# Add the project root directory to the Python path
project_root = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, project_root)

from src import DataLoader, QueryEngine, CLI

def display_results(results, result_count, result_size):
    print(f"\nQuery Results:")
    print(f"Total rows: {result_count}")
    print(f"Result size: {result_size / 1024:.2f} KB")

    if result_count > 100:
        user_input = input("The result has more than 100 rows. Would you like to display the first 100 rows? (y/n): ")
        if user_input.lower() == 'y':
            for row in results[:100]:
                print(row)
        else:
            print("Results not displayed due to large size.")
    else:
        for row in results:
            print(row)

def main():
    parser = argparse.ArgumentParser(description='CSV Query Engine')
    parser.add_argument('file_path', type=str, help='Path to the CSV file')
    args = parser.parse_args()

    print(f"Debug: Attempting to load file: {args.file_path}")  # Debug print
    start_time = time.time()
    data_loader = DataLoader(args.file_path)
    
    try:
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

        while True:
            query = input("Enter your SQL query (or 'exit' to quit): ")
            if query.lower() == 'exit':
                break

            try:
                results, result_count, result_size = query_engine.execute_query(query)
                display_results(results, result_count, result_size)
            except Exception as e:
                print(f"An error occurred: {str(e)}")
                print("Traceback:")
                traceback.print_exc()

    except KeyboardInterrupt:
        print("\nData loading interrupted by user. Exiting...")
    except Exception as e:
        print(f"An error occurred: {str(e)}")
        print("Traceback:")
        traceback.print_exc()

if __name__ == '__main__':
    main()