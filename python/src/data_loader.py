import csv
from .data_structure import ColumnStore

class DataLoader:
    def __init__(self, file_path):
        self.file_path = file_path

    def load_data(self):
        data = ColumnStore()
        with open(self.file_path, 'r', newline='', encoding='utf-8-sig') as csvfile:
            reader = csv.DictReader(csvfile)
            columns = [col.strip() for col in reader.fieldnames]
            
            # Read the first row to infer data types
            first_row = next(reader)
            column_types = self.infer_column_types(first_row)

            # print("Debug: Inferred column types:")
            # for col, type_ in column_types.items():
            #     print(f"  {col}: {type_.__name__}")
            
            # Add the first row
            converted_row = {k.strip(): self.convert_value(v, column_types[k.strip()]) for k, v in first_row.items()}
            data.add_row(converted_row)

            # print("Debug: First row data types:")
            # for col, value in converted_row.items():
            #     print(f"  {col}: {type(value).__name__}")


            # Add the rest of the rows
            for row in reader:
                cleaned_row = {k.strip(): self.convert_value(v, column_types[k.strip()]) for k, v in row.items()}
                data.add_row(cleaned_row)
        
        return data, columns

    def infer_column_types(self, row):
        column_types = {}
        for key, value in row.items():
            column_types[key.strip()] = self.infer_type(value)
        return column_types

    @staticmethod
    def infer_type(value):
        try:
            int(value)
            return int
        except ValueError:
            try:
                float(value)
                return float
            except ValueError:
                return str

    @staticmethod
    def convert_value(value, type_):
        if value == '':
            return None
        try:
            return type_(value)
        except ValueError:
            return value  # If conversion fails, return as string