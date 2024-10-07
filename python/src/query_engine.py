import re

class QueryEngine:
    def __init__(self, data, columns):
        self.data = data
        self.columns = columns

    def parse_condition(self, condition):
        match = re.match(r'(?:"([^"]+)"|(\w+))\s*([=<>]+)\s*(?:"([^"]*)"|(\S+))', condition)
        if match:
            column = match.group(1) or match.group(2)
            column = column.strip('"')
            operator = match.group(3)
            value = match.group(4) or match.group(5)
            value = value.strip('"')
            return column, operator, value
        return None, None, None

    def evaluate_condition(self, row, column, operator, value):
        if column not in self.columns:
            return False
        col_index = self.columns.index(column)
        row_value = row[col_index]
        
        if operator == '=':
            return row_value.strip().lower() == value.strip().lower()
        elif operator in ['>', '<']:
            try:
                row_float = float(row_value) if row_value else 0
                value_float = float(value)
                return row_float > value_float if operator == '>' else row_float < value_float
            except ValueError:
                return False
        return False

    def perform_calculation(self, row, calculation):
        for col in self.columns:
            if col in calculation:
                col_index = self.columns.index(col)
                value = row[col_index]
                calculation = calculation.replace(col, value)
        try:
            return eval(calculation)
        except Exception as e:
            print(f"Error in calculation: {e}")
            return None

    def execute_query(self, select_columns, conditions, order_by=None, limit=None):
        result = []
        for row_index, row in enumerate(self.data):
            if self.evaluate_conditions(row, conditions):
                result_row = {'_row_index': row_index}
                for col in select_columns:
                    if ' AS ' in col:
                        col_name, alias = col.split(' AS ')
                        col_name = col_name.strip()
                        alias = alias.strip().strip('"')
                        if '(' in col_name:  # It's a calculation
                            calc_result = self.perform_calculation(row, col_name)
                            result_row[alias] = calc_result
                        else:
                            result_row[alias] = self.get_column_value(row, col_name)
                    else:
                        result_row[col] = self.get_column_value(row, col)
                result.append(result_row)
        
        if order_by:
            reverse = order_by.endswith('DESC')
            order_col = order_by.split()[0]
            result.sort(key=lambda x: self.safe_numeric_convert(x[order_col]), reverse=reverse)
        
        if limit:
            result = result[:int(limit)]
        
        print(f"Query result count: {len(result)}")
        return result

    def safe_numeric_convert(self, value):
        try:
            return float(value)
        except ValueError:
            return value

    def sort_key(self, value):
        if isinstance(value, (int, float)):
            return value
        if isinstance(value, str):
            value = value.strip()
            if value == '':
                return float('-inf')  # Place empty strings at the beginning when sorting
            try:
                return float(value)
            except ValueError:
                return value
        return float('-inf')

    def evaluate_conditions(self, row, conditions):
        if not conditions:
            return True
        or_conditions = [cond.strip() for cond in ' '.join(conditions).split('OR')]
        return any(all(self.evaluate_condition(row, *self.parse_condition(c)) for c in and_cond.split('AND')) for and_cond in or_conditions)

    def get_column_value(self, row, column):
        if column in self.columns:
            return row[self.columns.index(column)]
        elif column.strip('"') in self.columns:
            return row[self.columns.index(column.strip('"'))]
        else:
            print(f"Column '{column}' not found in {self.columns}")
            return None

    def debug_print_data(self, limit=10):
        print(f"Columns: {self.columns}")
        print(f"First {limit} rows of data:")
        for i, row in enumerate(self.data[:limit]):
            print(f"Row {i}: {row}")
        print(f"Total number of rows: {len(self.data)}")