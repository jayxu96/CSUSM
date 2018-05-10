
Name: Runyu Xu
Date: 02/15/2017
Course: CS433 - Operating Systems
Assignment: 1




Description:

This program is an implementation of a priority queue of processes which contains two tests. The priority queue implements the heap data structure.


In Test 1, the program tests the implementation of the priority queue by simulating insertion and deletion of processes in a CPU.


In Test 2, the program tests the performance of the implementation by repeatedly add or remove processes to the ReadyQueue.


All Files:
     Source:
        Main.cpp              -the driver for the assignment
        ReadyQueue.cpp        -the data structure for the ready queue using a priority queue implement heap structure 
        PCB.cpp               -the data structure of a Process Control Block
        PCB_Table.cpp         -the data structure for PCB Table.
    


    Headers:
	Main.h
	ReadyQueue.h
	PCB.h
	PCB_Table.h



How to run:

To run this program, use the MakeFile which will compile the source files and create an executable called output.