CXX = mpic++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -march=native -fopenmp
LDFLAGS = -fopenmp

# Update these paths to match your OpenMPI installation 
OPENMPI_INCLUDE = -I/usr/lib/x86_64-linux-gnu/openmpi/include
OPENMPI_LIB = -L/usr/lib/x86_64-linux-gnu/openmpi/lib

# Uncomment and use these if you need to specify custom OpenMPI paths
# INCLUDES = -I$(OPENMPI_INCLUDE)
# LIBS = -L$(OPENMPI_LIB)

SRC_DIR = cPlusPlus/src
BUILD_DIR = cPlusPlus/build
BIN_DIR = cPlusPlus/bin

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Binary name
TARGET = $(BIN_DIR)/query_engine

# Default number of processes for MPI
NP ?= 4

# Default input file
INPUT_FILE ?= /mnt/c/Users/tript/IdeaProjects/CMPE275-Mini1/data/data_1.csv

# Phony targets
.PHONY: all clean run debug

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compiling
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create directories
$(BIN_DIR) $(BUILD_DIR):
	mkdir -p $@

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Run the program
run: $(TARGET)
	mpirun -np $(NP) $(TARGET) $(INPUT_FILE)

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: clean $(TARGET)

# Include dependencies
-include $(OBJS:.o=.d)

# Generate dependency files
$(BUILD_DIR)/%.d: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -MM -MT '$(@:.d=.o)' $< > $@
