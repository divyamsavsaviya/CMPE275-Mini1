# src/data/csv_reader.py
import csv
from src.data.csv_row import CSVRow

class CSVReader:
    def __init__(self, filename):
        self.filename = filename

    def read_csv(self):
        data = []
        with open(self.filename, 'r', encoding='utf-8-sig') as file:
            reader = csv.DictReader(file)
            for row in reader:
                if '\ufeff"Country Name"' in row:
                    row['Country Name'] = row.pop('\ufeff"Country Name"')
                csv_row = CSVRow(**row)
                data.append(csv_row)
        return data