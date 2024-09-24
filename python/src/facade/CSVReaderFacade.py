# src/facade/csv_reader_facade.py
from src.data.csv_reader import CSVReader

class CSVReaderFacade:
    def __init__(self, filename):
        self.reader = CSVReader(filename)
        self.data = self.reader.read_csv()
        self.indexed_data = {row.Country_Code: row for row in self.data}

    def get_all_data(self):
        return self.data

    def get_by_country_code(self, country_code):
        return self.indexed_data.get(country_code)

    def range_query(self, field, min_value, max_value):
        return [row for row in self.data if min_value <= getattr(row, field) <= max_value]