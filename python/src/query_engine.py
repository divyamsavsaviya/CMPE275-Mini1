import re
import time
import psutil
from functools import lru_cache
from concurrent.futures import ThreadPoolExecutor
from multiprocessing import Pool, cpu_count
from collections import defaultdict
import threading
import functools

class QueryEngine:
    def __init__(self, data, columns, index_columns=None):
        self.data = data
        self.columns = columns
        self.column_index = {col.strip('"'): idx for idx, col in enumerate(columns)}
        self.column_map = {col.lower(): col for col in columns}
        self.indexes = {}
        if index_columns:
            self.build_indexes(index_columns)
        self.query_cache = LRUCache(100)
        self.thread_count = cpu_count() * 2
        self.thread_pool = ThreadPoolExecutor(max_workers=cpu_count() * 2)
        print(f"Debug: Initialized with {self.thread_count} threads")
        self.process_pool = Pool(processes=cpu_count())
        self.lock = threading.Lock()

    def build_indexes(self, columns_to_index):
        for column in columns_to_index:
            if column in self.column_map:
                self.indexes[column] = defaultdict(list)
                column_data = self.data.get_column(self.column_map[column])
                for i, value in enumerate(column_data):
                    self.indexes[column][value].append(i)
            else:
                print(f"Warning: Column '{column}' not found in the dataset. Skipping index creation.")

    def execute_query(self, select_columns, conditions, order_by=None, limit=None):
        cache_key = (tuple(select_columns), tuple(conditions), order_by, limit)
        cached_result = self.query_cache.get(cache_key)
        if cached_result:
            return cached_result

        print(f"Debug: Original select_columns = {select_columns}")
        mapped_select_columns = [self.column_map.get(col.lower().strip('"'), col) for col in select_columns]
        print(f"Debug: Mapped select_columns = {mapped_select_columns}")

        start_time = time.time()
        process = psutil.Process()
        start_cpu_time = process.cpu_times()

        filtered_data = self.parallel_filter_data(conditions)
        print(f"Debug: Filtered data length: {len(filtered_data)}")

        selected_data = self.parallel_select_columns(filtered_data, mapped_select_columns)
        print(f"Debug: Selected data: {selected_data[:5]}")

        result = [{col: row[mapped_col] for col, mapped_col in zip(select_columns, mapped_select_columns)} for row in selected_data]

        if order_by:
            result = self.parallel_sort(result, order_by)

        if limit:
            result = result[:int(limit)]

        end_cpu_time = process.cpu_times()
        end_time = time.time()

        execution_time = end_time - start_time
        cpu_usage = (end_cpu_time.user - start_cpu_time.user) + (end_cpu_time.system - start_cpu_time.system)
        memory_usage = process.memory_info().rss / 1024 / 1024

        print(f"\nQuery Performance Metrics:")
        print(f"Execution Time: {execution_time:.4f} seconds")
        print(f"CPU Time: {cpu_usage:.4f} seconds")
        print(f"Memory Usage: {memory_usage:.2f} MB")

        self.query_cache.put(cache_key, result)
        return result

    def parallel_filter_data(self, conditions):
        if not conditions:
            return range(len(self.data))

        # Check if we can use an index for any of the conditions
        indexed_condition = self.find_indexed_condition(conditions)
        if indexed_condition:
            return self.filter_with_index(indexed_condition, conditions)

        # If no index can be used, proceed with parallel filtering
        chunk_size = max(1, len(self.data) // (cpu_count() * 4))
        chunks = [range(i, min(i+chunk_size, len(self.data))) for i in range(0, len(self.data), chunk_size)]
        chunk_count = len(chunks)
    
        print(f"Debug: Filtering data using {chunk_count} chunks")

        futures = []
        for chunk in chunks:
            futures.append(self.thread_pool.submit(self.filter_chunk, chunk, conditions))

        results = []
        for future in futures:
            results.extend(future.result())

        return results

    def find_indexed_condition(self, conditions):
        for condition in conditions:
            column, operator, value = self.parse_condition(condition)
            if column in self.indexes and operator == '=':
                return column, value
        return None

    def filter_with_index(self, indexed_condition, all_conditions):
        column, value = indexed_condition
        initial_set = set(self.indexes[column].get(value, []))
        
        if len(all_conditions) == 1:
            return list(initial_set)
        
        # Apply remaining conditions
        result = []
        for i in initial_set:
            row = self.data.get_row(i)
            if self.evaluate_conditions(row, all_conditions):
                result.append(i)
        return result

    def filter_chunk(self, chunk, conditions):
        result = []
        for i in chunk:
            row = self.data.get_row(i)
            if self.evaluate_conditions(row, conditions):
                result.append(i)
        print(f"Debug: Filter chunk result length: {len(result)}")
        return result

    def evaluate_conditions(self, row, conditions):
        if not conditions:
            return True
        
        or_conditions = [cond.strip() for cond in ' '.join(conditions).split('OR')]
        
        for or_condition in or_conditions:
            and_conditions = [cond.strip() for cond in or_condition.split('AND')]
            if all(self.evaluate_single_condition(row, cond) for cond in and_conditions):
                return True
        
        return False

    def evaluate_single_condition(self, row, condition):
        column, operator, value = self.parse_condition(condition)
        actual_column = self.column_map.get(column.lower().strip('"'), column)
        row_value = row.get(actual_column)
        column_type = self.data.get_column_type(actual_column)
        
        if row_value is None:
            return False

        try:
            typed_value = column_type(value.strip("'\""))
        except ValueError:
            return False

        if operator == '=':
            return row_value == typed_value
        elif operator == '>':
            return row_value > typed_value
        elif operator == '<':
            return row_value < typed_value
        return False

    def parallel_select_columns(self, row_indices, select_columns):
        chunk_size = max(1, len(row_indices) // (cpu_count() * 4))
        chunks = [row_indices[i:i+chunk_size] for i in range(0, len(row_indices), chunk_size)]

        futures = []
        for chunk in chunks:
            futures.append(self.thread_pool.submit(self.select_columns_chunk, chunk, select_columns))

        results = []
        for future in futures:
            results.extend(future.result())

        return results

    def select_columns_chunk(self, row_indices, select_columns):
        return [{col: self.data.get_column(col)[i] for col in select_columns} for i in row_indices]

    def parallel_sort(self, data, order_by):
        reverse = order_by.endswith('DESC')
        order_col = order_by.split()[0].strip('"')

        chunk_size = max(1, len(data) // cpu_count())
        chunks = [data[i:i+chunk_size] for i in range(0, len(data), chunk_size)]

        sorted_chunks = self.process_pool.map(functools.partial(self.sort_chunk, order_col=order_col, reverse=reverse), chunks)

        return self.merge_sorted_chunks(sorted_chunks, order_col, reverse)

    @classmethod
    def sort_chunk(cls, chunk, order_col, reverse):
        return sorted(chunk, key=lambda item: cls.safe_numeric_convert(item[order_col]), reverse=reverse)

    def merge_sorted_chunks(self, sorted_chunks, order_col, reverse):
        result = []
        indices = [0] * len(sorted_chunks)

        while True:
            min_chunk = None
            min_index = -1

            for i, chunk in enumerate(sorted_chunks):
                if indices[i] < len(chunk):
                    if min_chunk is None or self.compare_items(chunk[indices[i]], min_chunk, order_col, reverse):
                        min_chunk = chunk[indices[i]]
                        min_index = i

            if min_index == -1:
                break

            result.append(min_chunk)
            indices[min_index] += 1

        return result

    @staticmethod
    def compare_items(item1, item2, order_col, reverse):
        val1 = QueryEngine.safe_numeric_convert(item1[order_col])
        val2 = QueryEngine.safe_numeric_convert(item2[order_col])
        if reverse:
            return val1 > val2 if val1 != val2 else item1[order_col] > item2[order_col]
        else:
            return val1 < val2 if val1 != val2 else item1[order_col] < item2[order_col]


    @staticmethod
    def safe_numeric_convert(value):
        if isinstance(value, (int, float)):
            return value
        if isinstance(value, str):
            value = value.replace(',', '')  # Remove commas from number strings
            try:
                return float(value)
            except ValueError:
                return float('-inf')  # Return negative infinity for non-numeric strings
        return float('-inf')  # Return negative infinity for other types

    @lru_cache(maxsize=1000)
    def parse_condition(self, condition):
        pattern = r'(?:"([^"]+)"|(\w+))\s*([=<>]+)\s*(?:\'([^\']*)\'|"([^"]*)"|([\w.]+))'
        match = re.match(pattern, condition)
        if match:
            column = match.group(1) or match.group(2)
            operator = match.group(3)
            value = match.group(4) or match.group(5) or match.group(6)
            return column.strip('"'), operator, value
        return None, None, None

    def debug_print_data(self, limit=10):
        # print(f"Columns: {self.columns}")
        # print(f"First {limit} rows of data:")
        # for i in range(min(limit, len(self.data))):
        #     row = self.data.get_row(i)
        #     print(f"Row {i}: {row}")
        print(f"Total number of rows: {len(self.data)}")

class LRUCache:
    def __init__(self, capacity):
        self.capacity = capacity
        self.cache = {}
        self.lru = []

    def get(self, key):
        if key in self.cache:
            self.lru.remove(key)
            self.lru.append(key)
            return self.cache[key]
        return None

    def put(self, key, value):
        if key in self.cache:
            self.lru.remove(key)
        elif len(self.cache) >= self.capacity:
            oldest = self.lru.pop(0)
            del self.cache[oldest]
        self.cache[key] = value
        self.lru.append(key)