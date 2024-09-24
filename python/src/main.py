# src/main.py
import time
import psutil
from memory_profiler import memory_usage
from src.api.data_api import DataAPI

def measure_performance(func):
    def wrapper(*args, **kwargs):
        start_time = time.perf_counter()
        process = psutil.Process()
        start_cpu_times = process.cpu_times()
        mem_usage_before = memory_usage(-1, interval=0.1, timeout=1)

        result = func(*args, **kwargs)

        mem_usage_after = memory_usage(-1, interval=0.1, timeout=1)
        end_cpu_times = process.cpu_times()
        end_time = time.perf_counter()

        elapsed_time = round(end_time - start_time, 5)
        cpu_time_user = round(end_cpu_times.user - start_cpu_times.user, 5)
        cpu_time_system = round(end_cpu_times.system - start_cpu_times.system, 5)
        mem_usage = round(mem_usage_after[0] - mem_usage_before[0], 5)

        print(f'Finished in {elapsed_time} second(s)')
        print(f'User CPU time: {cpu_time_user} second(s)')
        print(f'System CPU time: {cpu_time_system} second(s)')
        print(f'Memory usage: {mem_usage} MiB')

        return result
    return wrapper

@measure_performance
def main():
    api = DataAPI('data/data_1.csv')
    all_data = api.get_all_data()
    print(all_data[0])

if __name__ == "__main__":
    main()