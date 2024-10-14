#pragma once
#include <cstddef>
#include <cstdlib>

class MemoryPool {
public:
    MemoryPool(size_t size);
    void* allocate(size_t size);
    void reset();
    ~MemoryPool();

private:
    char* memory;
    size_t used;
    size_t capacity;
};