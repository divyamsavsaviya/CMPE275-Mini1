CXX = mpic++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
LDFLAGS = -lmpi

# Update these paths to match your OpenMPI installation 
OPENMPI_INCLUDE = -I/usr/lib/x86_64-linux-gnu/openmpi/include
OPENMPI_LIB = -L/usr/lib/x86_64-linux-gnu/openmpi/lib

# Uncomment and use these if you need to specify custom OpenMPI paths
# INCLUDES = -I$(OPENMPI_INCLUDE)
# LIBS = -L$(OPENMPI_LIB)

SRC_DIR = cPlusPlus/src
API_DIR = $(SRC_DIR)/api
DATA_DIR = $(SRC_DIR)/data
BUILD_DIR = cPlusPlus/build

INCLUDES = -I$(API_DIR) -I$(DATA_DIR)

MAIN_SRC = $(SRC_DIR)/main.cpp
SRCS = $(MAIN_SRC) $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(API_DIR)/*.cpp) $(wildcard $(DATA_DIR)/*.cpp)
DEPS = $(OBJS:.o=.d)

OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

EXEC = $(BUILD_DIR)/query_engine

.PHONY: all clean run

all: $(EXEC)

$(EXEC): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

run: $(EXEC)
	mpirun -np 8 ./$(EXEC) path/to/your/csv/file.csv

-include $(DEPS)