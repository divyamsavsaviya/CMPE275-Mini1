import csv
import mmap
import os
import multiprocessing
from io import StringIO
from .data_structure import ColumnStore
import time

class DataLoader:
    def __init__(self, file_path):
        self.file_path = file_path
        self.chunk_size = 1024 * 1024 * 10  # 10MB chunks
        self.num_processes = multiprocessing.cpu_count()

    def load_data(self):
        try:
            print("Starting data loading process...")
            start_time = time.time()
            file_size = os.path.getsize(self.file_path)
            chunk_size = max(self.chunk_size, file_size // (self.num_processes * 10))
            
            print(f"File size: {file_size / (1024*1024):.2f} MB")
            print(f"Using {self.num_processes} processes")
            
            with open(self.file_path, 'r+b') as f:
                mm = mmap.mmap(f.fileno(), 0)
                columns = self._get_columns(mm)
                print(f"Detected {len(columns)} columns")
                column_types = self._infer_column_types(mm, columns)
                print("Column types inferred")

                chunks = self._split_file(mm, chunk_size)
                print(f"File split into {len(chunks)} chunks")
                
                with multiprocessing.Pool(processes=self.num_processes) as pool:
                    results = []
                    for i, result in enumerate(pool.imap_unordered(self._process_chunk_wrapper, 
                                               [(chunk, column_types, columns, self.file_path) for chunk in chunks])):
                        results.append(result)
                        print(f"Processed chunk {i+1}/{len(chunks)}", end='\r')

            print("\nMerging chunks...")
            data = ColumnStore()
            for chunk_data in results:
                for col, values in chunk_data.items():
                    data.columns[col].extend(values)
                data.row_count += len(chunk_data[columns[0]])

            end_time = time.time()
            print(f"Data loading completed in {end_time - start_time:.2f} seconds")
            return data, columns
        except Exception as e:
            print(f"Error in load_data: {str(e)}")
            raise

    def _get_columns(self, mm):
        try:
            first_line = mm.readline().decode('utf-8').strip()
            return [col.strip() for col in first_line.split(',')]
        except Exception as e:
            print(f"Error in _get_columns: {str(e)}")
            raise

    def _infer_column_types(self, mm, columns):
        try:
            mm.seek(0)
            reader = csv.reader(StringIO(mm.readline().decode('utf-8')))  # Skip header
            first_row = next(csv.reader(StringIO(mm.readline().decode('utf-8'))))
            return {col: self._infer_type(value) for col, value in zip(columns, first_row)}
        except Exception as e:
            print(f"Error in _infer_column_types: {str(e)}")
            raise

    @staticmethod
    def _infer_type(value):
        try:
            int(value)
            return int
        except ValueError:
            try:
                float(value)
                return float
            except ValueError:
                return str

    def _split_file(self, mm, chunk_size):
        chunks = []
        file_size = mm.size()
        start = mm.find(b'\n') + 1  # Skip header
        while start < file_size:
            end = mm.find(b'\n', start + chunk_size)
            if end == -1:  # Last chunk
                end = file_size
            chunks.append((start, end))
            start = end + 1
        return chunks

    @staticmethod
    def _process_chunk_wrapper(args):
        return DataLoader._process_chunk(*args)

    @staticmethod
    def _process_chunk(chunk, column_types, columns, file_path):
        try:
            start, end = chunk
            chunk_data = {col: [] for col in columns}
            
            with open(file_path, 'r+b') as f:
                mm = mmap.mmap(f.fileno(), 0)
                mm.seek(start)
                chunk_content = mm.read(end - start).decode('utf-8')
                
                for row in csv.reader(StringIO(chunk_content)):
                    for col, value in zip(columns, row):
                        chunk_data[col].append(DataLoader._convert_value(value, column_types[col]))
            
            return chunk_data
        except Exception as e:
            print(f"Error in _process_chunk: {str(e)}")
            raise

    @staticmethod
    def _convert_value(value, type_):
        if value == '':
            return None
        try:
            return type_(value)
        except ValueError:
            return value  # If conversion fails, return as string