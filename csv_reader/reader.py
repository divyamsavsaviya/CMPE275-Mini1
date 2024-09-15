import csv

def read_chunk(filename, start, end):
    with open(filename, 'r') as file:
        file.seek(start)
        lines = file.read(end - start).splitlines()
        reader = csv.reader(lines)
        return list(reader)