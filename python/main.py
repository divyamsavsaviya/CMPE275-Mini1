import os
import argparse
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

    # Load data
    loader = DataLoader(csv_path)
    data, columns = loader.load()

    # Initialize query engine
    query_engine = QueryEngine(data, columns)

    # Run CLI
    cli = CLI(query_engine)
    cli.run()

if __name__ == '__main__':
    main()