import re

class CLI:
    def __init__(self, query_engine):
        self.query_engine = query_engine

    def run(self):
        # Debug: Print data information
        self.query_engine.debug_print_data()

        while True:
            print("\nEnter your query (or 'exit' to quit):")
            print("Format: SELECT column1,column2,... WHERE condition1 AND condition2 ... ORDER BY column [ASC|DESC] LIMIT n")
            query = input("> ")
            
            if query.lower() == 'exit':
                break

            try:
                select_part, where_part, order_by, limit = self.parse_query(query)

                select_columns = [col.strip() for col in select_part.replace('SELECT', '').strip().split(',')]
                conditions = [cond.strip() for cond in where_part.strip().split('AND')] if where_part else []

                result = self.query_engine.execute_query(select_columns, conditions, order_by, limit)
                self.display_results(result, select_columns)
            except Exception as e:
                print(f"An error occurred: {str(e)}")

    def parse_query(self, query):
        select_pattern = r'SELECT\s+(.*?)\s*(?:WHERE|ORDER BY|LIMIT|$)'
        where_pattern = r'WHERE\s+(.*?)\s*(?:ORDER BY|LIMIT|$)'
        order_by_pattern = r'ORDER BY\s+(.*?)\s*(?:LIMIT|$)'
        limit_pattern = r'LIMIT\s+(\d+)'

        select_match = re.search(select_pattern, query, re.IGNORECASE)
        where_match = re.search(where_pattern, query, re.IGNORECASE)
        order_by_match = re.search(order_by_pattern, query, re.IGNORECASE)
        limit_match = re.search(limit_pattern, query, re.IGNORECASE)

        select_part = select_match.group(1) if select_match else ''
        where_part = where_match.group(1) if where_match else ''
        order_by = order_by_match.group(1) if order_by_match else None
        limit = limit_match.group(1) if limit_match else None

        return select_part, where_part, order_by, limit

    def display_results(self, results, select_columns):
        if not results:
            print("No results found.")
            return

        # Determine the columns to display
        display_columns = select_columns if select_columns[0] != '*' else results[0].keys()
        if '_row_index' in display_columns:
            display_columns.remove('_row_index')

        # Print header
        header = "Row | " + " | ".join(display_columns)
        print(header)
        print("-" * len(header))

        # Print rows
        for row in results:
            row_index = row.get('_row_index', 'N/A')
            row_values = []
            for col in display_columns:
                value = row.get(col, '')
                if isinstance(value, (int, float)):
                    value = f"{value:.2f}" if isinstance(value, float) else f"{value}"
                row_values.append(str(value))
            print(f"{row_index:3} | " + " | ".join(row_values))

        print(f"\nTotal results: {len(results)}")