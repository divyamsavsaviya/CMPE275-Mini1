import csv
from src.csv_row import CSVRow

class CSVReaderFacade:
    def __init__(self, filename):
        self.filename = filename
        self.data = []
        self.indexed_data = {}

    def read_csv(self):
        print(f"Reading CSV file: {self.filename}")
        with open(self.filename, 'r', encoding='utf-8-sig') as file:
            reader = csv.DictReader(file)
            for row in reader:
                # Strip BOM character from the first key if present
                if '\ufeff"Country Name"' in row:
                    row['Country Name'] = row.pop('\ufeff"Country Name"')
                csv_row = CSVRow(**row)
                self.data.append(csv_row)
                self.indexed_data[csv_row.Country_Code] = csv_row
        print("Finished reading CSV file.")
        return self.data