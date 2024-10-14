#pragma once
#include <string>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>

class MemoryMappedFile {
private:
    int fd;
    void* mapped_data;
    size_t file_size;

public:
    MemoryMappedFile(const std::string& filename) {
        fd = open(filename.c_str(), O_RDONLY);
        if (fd == -1) {
            throw std::runtime_error("Failed to open file");
        }
        file_size = lseek(fd, 0, SEEK_END);
        mapped_data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (mapped_data == MAP_FAILED) {
            close(fd);
            throw std::runtime_error("Failed to map file");
        }
    }

    ~MemoryMappedFile() {
        munmap(mapped_data, file_size);
        close(fd);
    }

    void* getData() const { return mapped_data; }
    size_t getSize() const { return file_size; }
};