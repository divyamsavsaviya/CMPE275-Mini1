# src/api/data_api.py
from src.facade.CSVReaderFacade import CSVReaderFacade

class DataAPI:
    def __init__(self, filename):
        self.facade = CSVReaderFacade(filename)

    def get_all_data(self):
        return self.facade.get_all_data()