##################################################
# Runyu Xu
# CS433 - Operating Systems
# Assignment 1 - Priority Queue of Processes
# 02/15/2018
##################################################

# use g++ for compiling c++ code 
CC = g++

# compilation flags: -g for debugging. -02 for optimized code
CFLAGS = -g -std=c++11 -o2
# linked libraries
LIB = -lm
# link flags
LDFLAGS = -L.
# target executable (output)
PROG = ReadyQueueTest
# .c or .cpp source files
SRC = main.cpp PCB.cpp PCB_Table.cpp ReadyQueue.cpp
# object files for the target. Add more to this and next lines if there are more than one source files.
OBJ = $(SRC:.cpp=.o)

all : $(PROG)

$(PROG): $(OBJ) 
	$(CC) -o $(PROG) $(OBJ) $(LDFLAGS) $(LIB)

.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@

# cleanup
clean:
	/bin/rm -f *.o $(PROG)

# DO NOT DELETE
