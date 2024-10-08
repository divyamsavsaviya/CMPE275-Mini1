from collections import defaultdict

class ColumnStore:
    def __init__(self):
        self.columns = defaultdict(list)
        self.column_types = {}
        self.row_count = 0

    def add_row(self, row):
        for col, value in row.items():
            self.columns[col].append(value)
            if col not in self.column_types:
                self.column_types[col] = type(value)
        self.row_count += 1
        if self.row_count % 1000 == 0:
            print(f"Debug: Added {self.row_count} rows to ColumnStore")  # Debug print

    def get_column(self, column_name):
        return self.columns[column_name]

    def get_row(self, row_index):
        return {col: values[row_index] for col, values in self.columns.items()}

    def get_column_type(self, column_name):
        return self.column_types.get(column_name, str)

    def __len__(self):
        return self.row_count