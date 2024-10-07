# Compiler
CXX = g++
CXXFLAGS = -I./include -std=c++17

# Source files
SRCS = src/CPlus/main.cpp src/CPlus/api/DataApi.cpp src/CPlus/api/PerformanceMeasurement.cpp src/CPlus/data/CSVReader.cpp src/CPlus/data/CSVReaderFacade.cpp src/CPlus/data/CSVRow.cpp src/CPlus/api/CLI.cpp src/CPlus/api/QueryEngine.h

# Object files
OBJS = $(SRCS:.cpp=.o)

# Output executable
TARGET = myprogram

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Compile .cpp files to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
