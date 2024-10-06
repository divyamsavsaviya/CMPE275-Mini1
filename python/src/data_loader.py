import csv

class DataLoader:
    def __init__(self, file_path):
        self.file_path = file_path
        self.data = []
        self.columns = []

    def load(self):
        with open(self.file_path, 'r', encoding='utf-8-sig') as file:
            reader = csv.reader(file)
            self.columns = next(reader)
            for row in reader:
                self.data.append(row)
        
        # Remove any remaining quotes from column names
        self.columns = [col.strip('"') for col in self.columns]
        
        print(f"Columns: {self.columns}")
        print(f"Number of rows: {len(self.data)}")
        print(f"First row: {self.data[0]}")
        print(f"Column index for 'Country Name': {self.columns.index('Country Name') if 'Country Name' in self.columns else 'Not found'}")
        print(f"Sample 'Country Name' values: {[row[self.columns.index('Country Name')] for row in self.data[:5] if 'Country Name' in self.columns]}")
        
        return self.data, self.columns