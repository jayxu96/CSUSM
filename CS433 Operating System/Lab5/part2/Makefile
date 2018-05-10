##################################################
# Runyu Xu
# CS433 - Operating Systems
# Assignment 5 - Memory part1
# 05/06/2018
##################################################

# use g++ for compiling c++ code 
CC = g++ -lpthread

# compilation flags: -g for debugging. -02 for optimized code
CFLAGS = -c  -o2
# linked libraries
LIB = -lm
# link flags
LDFLAGS = -L.
SOURCES = part2.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = part2

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *o $(EXECUTABLE)
