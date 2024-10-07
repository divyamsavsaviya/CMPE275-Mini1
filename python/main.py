import os
import argparse
import time
import psutil
from memory_profiler import memory_usage
from src.data_loader import DataLoader
from src.query_engine import QueryEngine
from src.cli import CLI

def main():
    # Set up argument parser
    parser = argparse.ArgumentParser(description="CSV Query Engine")
    parser.add_argument("csv_path", help="Path to the CSV file")
    args = parser.parse_args()

    # Get the full path to the CSV file
    csv_path = os.path.abspath(args.csv_path)

    # Check if the file exists
    if not os.path.exists(csv_path):
        print(f"Error: CSV file not found at {csv_path}")
        print("Please ensure the path to your CSV file is correct.")
        return

    start_time = time.time()
    process = psutil.Process()
    start_cpu_time = process.cpu_times()

    # Load data
    loader = DataLoader(csv_path)
    data, columns = loader.load()

    # Initialize query engine
    query_engine = QueryEngine(data, columns)

    # Run CLI
    cli = CLI(query_engine)
    cli.run()

    end_cpu_time = process.cpu_times()
    end_time = time.time()

    execution_time = end_time - start_time
    cpu_usage = (end_cpu_time.user - start_cpu_time.user) + (end_cpu_time.system - start_cpu_time.system)
    memory_usage = process.memory_info().rss / 1024 / 1024  # in MB

    print(f"\nOverall Performance Metrics:")
    print(f"Total Execution Time: {execution_time:.2f} seconds")
    print(f"CPU Time: {cpu_usage:.2f} seconds")
    print(f"Peak Memory Usage: {memory_usage:.2f} MB")

if __name__ == '__main__':
    main()