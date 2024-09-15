from csv_reader.facade import CSVReaderFacade
import time

if __name__ == "__main__":

    start = time.perf_counter();
    csv_reader = CSVReaderFacade('data/data_1.csv')
    data = csv_reader.read_csv()
    
    # for row in data:
    #     print(row)

    print(len(data))

    finish = time.perf_counter()
    print(f'Finished in {round(finish - start, 3)} second(s)')