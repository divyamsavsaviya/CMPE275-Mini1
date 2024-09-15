import os
import concurrent.futures
from .reader import read_chunk

class CSVReaderFacade:
    def __init__(self, filename, chunk_size=1024):
        self.filename = filename
        self.chunk_size = chunk_size

    def read_csv(self):
        file_size = os.path.getsize(self.filename)
        futures = []
        results = []

        with concurrent.futures.ThreadPoolExecutor() as executor:
            for start in range(0, file_size, self.chunk_size):
                end = min(start + self.chunk_size, file_size)
                futures.append(executor.submit(read_chunk, self.filename, start, end))

            for future in concurrent.futures.as_completed(futures):
                results.extend(future.result())

        return results