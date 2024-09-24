# tests/test_csv_reader.py
import unittest
from src.api.data_api import DataAPI

class TestCSVReaderFacade(unittest.TestCase):

    def setUp(self):
        self.api = DataAPI('data/sample.csv')

    def test_get_all_data(self):
        data = self.api.get_all_data()
        self.assertEqual(len(data), 2)

    def test_get_by_country_code(self):
        country_data = self.api.get_by_country_code('A')
        self.assertIsNotNone(country_data)
        self.assertEqual(country_data.Country_Name, 'Country A')

    def test_range_query(self):
        range_data = self.api.range_query('Year_1960', 100, 200)
        self.assertEqual(len(range_data), 1)
        self.assertEqual(range_data[0].Country_Code, 'A')

if __name__ == '__main__':
    unittest.main()