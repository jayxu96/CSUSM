//===================================================================
// Assignment: Priority Queue of Processes
// Author: Runyu Xu
// Compiler: g++
// File type: ReadyQueue header file
// Date: 02/15/2018
//===================================================================
//Description: This file is the header file for the ReadyQueue class,
//             which will create a queue of processes that are in the 
//             Ready state to be scheduled to run
//===================================================================
#pragma once
#include <iostream>
#include <vector>
#include "PCB.h"
using namespace std;

class ReadyQueue {
private:
		vector<PCB> heap; //create a heap to hold the priority queue
		// methods for heap
	int left(int parent); // get the left child 
	int right(int parent); // get the right child
	int parent(int child); // get the parent
	void heapUp(int index); // up a node to correct index
	void heapDown(int index); // down a node to correct index
	void deleteMin(); // delete the min element
	PCB getMin(); // get the value of the min element

public:
	ReadyQueue();    // Constructor
	~ReadyQueue();    // Destructor

	void insertProc(PCB process); // insert a process into the queue
	PCB removeHighestProc(); // remove and return the PCB with the highest
							 //priority from the queue
	int size(); // return the number of elements in the queue
	void displayQueue(); // display the IDs and priorities of processes 
	void reHeap(); // reheap the queue
	bool checkProc(PCB process); // check if a process is already in the list
};