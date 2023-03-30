# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra  -pedantic -std=c++20

# Source directories
SRCDIRS = $(wildcard */)

# Source files
SRCS = $(foreach dir,$(SRCDIRS),$(wildcard $(dir)*.cpp))

# Object files
OBJS = $(SRCS:.cpp=.o)

# Targets
.PHONY: all clean

all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o myprogram

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) myprogram
