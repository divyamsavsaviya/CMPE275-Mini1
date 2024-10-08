import time
import re

class CLI:
    def __init__(self, query_engine):
        self.query_engine = query_engine

    def run(self):
        self.query_engine.debug_print_data()

        while True:
            print("\nEnter your query (or 'exit' to quit):")
            print("Format: SELECT column1,column2,... WHERE condition1 AND condition2 ... ORDER BY column [ASC|DESC] LIMIT n")
            query = input("> ")
            
            if query.lower() == 'exit':
                break

            try:
                start_time = time.time()
                select_part, where_part, order_by, limit = self.parse_query(query)

                select_columns = [col.strip() for col in select_part.replace('SELECT', '').strip().split(',')]
                conditions = [cond.strip() for cond in where_part.strip().split('AND')] if where_part else []

                result = self.query_engine.execute_query(select_columns, conditions, order_by, limit)
                self.display_results(result, select_columns)
                end_time = time.time()
                print(f"Total query processing time: {end_time - start_time:.4f} seconds")
            except Exception as e:
                print(f"An error occurred: {str(e)}")

    def parse_query(self, query):
        select_pattern = r'SELECT\s+(.*?)\s+(?:WHERE|ORDER BY|LIMIT|$)'
        where_pattern = r'WHERE\s+(.*?)(?:\s+ORDER BY|\s+LIMIT|$)'
        order_by_pattern = r'ORDER BY\s+(.*?)(?:\s+LIMIT|$)'
        limit_pattern = r'LIMIT\s+(\d+)'

        select_match = re.search(select_pattern, query, re.IGNORECASE)
        where_match = re.search(where_pattern, query, re.IGNORECASE)
        order_by_match = re.search(order_by_pattern, query, re.IGNORECASE)
        limit_match = re.search(limit_pattern, query, re.IGNORECASE)

        select_part = select_match.group(1) if select_match else None
        where_part = where_match.group(1) if where_match else None
        order_by = order_by_match.group(1) if order_by_match else None
        limit = limit_match.group(1) if limit_match else None

        if not select_part:
            raise ValueError("Invalid query: SELECT statement is required")

        print(f"Debug: Parsed query - SELECT: {select_part}, WHERE: {where_part}, ORDER BY: {order_by}, LIMIT: {limit}")
        return select_part, where_part, order_by, limit

    def display_results(self, results, select_columns):
        print(f"Debug: Results to display: {results}")
        if not results:
            print("No results found.")
            return

        headers = select_columns
        print("Row | " + " | ".join(headers))
        print("-" * (sum(len(h) for h in headers) + 3 * len(headers) + 5))

        for i, row in enumerate(results):
            print(f"{i:3} | " + " | ".join(str(row.get(col, '')) for col in headers))

        print(f"\nTotal results: {len(results)}")