import csv
from .data_structure import ColumnStore

class DataLoader:
    def __init__(self, file_path):
        self.file_path = file_path

    def load_data(self):
        data = ColumnStore()
        with open(self.file_path, 'r', newline='', encoding='utf-8-sig') as csvfile:  # Changed encoding to 'utf-8-sig'
            reader = csv.DictReader(csvfile)
            columns = [col.strip() for col in reader.fieldnames]  # Strip whitespace from column names
            for row in reader:
                cleaned_row = {k.strip(): v for k, v in row.items()}  # Strip whitespace from keys
                data.add_row(cleaned_row)
        return data, columns