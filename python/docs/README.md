# CSV Query Engine

## How to Run the System

1. Setup:
   - Ensure Python 3.8+ is installed on your system.
   - Clone this repository: `git clone [repository-url]`
   - Navigate to the project directory: `cd TeamName_CSVQueryEngine`
   - Create a virtual environment: `python -m venv venv`
   - Activate the virtual environment:
     - On Windows: `venv\Scripts\activate`
     - On macOS and Linux: `source venv/bin/activate`
   - Install dependencies: `pip install -r requirements.txt`

2. Running the Query Engine:
   - From the project root, run: `python src/main.py [path-to-csv-file]`
   - You will be prompted to enter SQL-like queries.

3. Example Queries:
   - `SELECT column1, column2 WHERE condition1 = 'value' ORDER BY column3 DESC LIMIT 10`
   - `SELECT * WHERE column > 100 AND column2 = 'test'`

4. Running Tests:
   - From the project root, run: `python -m unittest discover tests`

## Project Structure

- `src/`: Contains the source code for the CSV query engine.
- `tests/`: Contains unit tests for the project components.
- `docs/`: Contains detailed documentation about the project architecture and performance.

For more detailed information about the architecture and design decisions, please refer to `docs/ARCHITECTURE.md`.