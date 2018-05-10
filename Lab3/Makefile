##################################################
# Runyu Xu
# CS433 - Operating Systems
# Assignment 1 - Priority Queue of Processes
# 02/15/2018
##################################################

# use g++ for compiling c++ code 
CC = g++

# compilation flags: -g for debugging. -02 for optimized code
CFLAGS = -c -std=c++11 -o2
# linked libraries
LIB = -lm
# link flags
LDFLAGS = -L.
SOURCES = main.cpp event.cpp process.cpp simulation.cpp FCFS.cpp SJF.cpp random.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = cpusim.exe

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *o $(EXECUTABLE)
