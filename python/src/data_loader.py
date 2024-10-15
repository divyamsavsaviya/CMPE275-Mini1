import csv
from .data_structure import ColumnStore
import threading
from queue import Queue
import multiprocessing

class DataLoader:
    def __init__(self, file_path):
        self.file_path = file_path
        self.chunk_size = 100000  # Adjust based on available memory
        self.num_threads = multiprocessing.cpu_count()

    def load_data(self):
        data = ColumnStore()
        columns = self._get_columns()
        column_types = self._infer_column_types()

        chunk_queue = Queue()
        result_queue = Queue()

        # Start worker threads
        threads = []
        for _ in range(self.num_threads):
            t = threading.Thread(target=self._process_chunk, args=(chunk_queue, result_queue, column_types))
            t.start()
            threads.append(t)

        # Read and enqueue chunks
        with open(self.file_path, 'r', newline='', encoding='utf-8-sig') as csvfile:
            reader = csv.DictReader(csvfile)
            chunk = []
            for row in reader:
                chunk.append(row)
                if len(chunk) == self.chunk_size:
                    chunk_queue.put(chunk)
                    chunk = []
            if chunk:
                chunk_queue.put(chunk)

        # Signal threads to finish
        for _ in range(self.num_threads):
            chunk_queue.put(None)

        # Wait for all threads to complete
        for t in threads:
            t.join()

        # Process results
        while not result_queue.empty():
            chunk_data = result_queue.get()
            for col, values in chunk_data.items():
                data.columns[col].extend(values)
            data.row_count += len(chunk_data[columns[0]])

        return data, columns

    def _get_columns(self):
        with open(self.file_path, 'r', newline='', encoding='utf-8-sig') as csvfile:
            reader = csv.reader(csvfile)
            return [col.strip() for col in next(reader)]

    def _infer_column_types(self):
        with open(self.file_path, 'r', newline='', encoding='utf-8-sig') as csvfile:
            reader = csv.reader(csvfile)
            next(reader)  # Skip header
            first_row = next(reader)
            return {col: self._infer_type(value) for col, value in zip(self._get_columns(), first_row)}

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

    def _process_chunk(self, chunk_queue, result_queue, column_types):
        while True:
            chunk = chunk_queue.get()
            if chunk is None:
                break

            chunk_data = {col: [] for col in column_types}
            for row in chunk:
                for col, value in row.items():
                    chunk_data[col].append(self._convert_value(value, column_types[col]))

            result_queue.put(chunk_data)

    @staticmethod
    def _convert_value(value, type_):
        if value == '':
            return None
        try:
            return type_(value)
        except ValueError:
            return value  # If conversion fails, return as string
