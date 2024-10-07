CC = g++
CFLAGS = -std=c++17 -Wall -I./cPlusPlus/src
SRCDIR = cPlusPlus/src
OBJDIR = obj

SOURCES = $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/api/*.cpp $(SRCDIR)/data/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
EXECUTABLE = myprogram

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Add explicit rules for each object file
$(OBJDIR)/main.o: $(SRCDIR)/main.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/api/QueryEngine.o: $(SRCDIR)/api/QueryEngine.cpp
	@mkdir -p $(OBJDIR)/api
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/api/CLI.o: $(SRCDIR)/api/CLI.cpp
	@mkdir -p $(OBJDIR)/api
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/data/CSVReader.o: $(SRCDIR)/data/CSVReader.cpp
	@mkdir -p $(OBJDIR)/data
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(EXECUTABLE)

.PHONY: all clean