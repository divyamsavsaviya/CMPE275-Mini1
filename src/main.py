from src.csv_reader import CSVReaderFacade
import time
import psutil
from memory_profiler import memory_usage

def measure_performance():
    # Start measuring time
    start_time = time.perf_counter()

    # Start measuring CPU usage
    process = psutil.Process()
    start_cpu_times = process.cpu_times()

    # Start measuring memory usage
    mem_usage_before = memory_usage(-1, interval=0.1, timeout=1)

    # Read CSV data
    csv_reader = CSVReaderFacade('data/data_1.csv')
    data = csv_reader.read_csv()

    # End measuring memory usage
    mem_usage_after = memory_usage(-1, interval=0.1, timeout=1)

    # End measuring CPU usage
    end_cpu_times = process.cpu_times()

    # End measuring time
    end_time = time.perf_counter()

    # Calculate metrics
    elapsed_time = round(end_time - start_time, 5)
    cpu_time_user = round(end_cpu_times.user - start_cpu_times.user, 5)
    cpu_time_system = round(end_cpu_times.system - start_cpu_times.system, 5)
    mem_usage = round(mem_usage_after[0] - mem_usage_before[0], 5)

    # Print the first few rows to verify the data
    for i, row in enumerate(data[:5]):  # Adjust the range as needed
        print(f"Row {i}: {row}\n")

    # Print performance metrics
    print(f'Finished in {elapsed_time} second(s)')
    print(f'User CPU time: {cpu_time_user} second(s)')
    print(f'System CPU time: {cpu_time_system} second(s)')
    print(f'Memory usage: {mem_usage} MiB')

if __name__ == "__main__":
    measure_performance()