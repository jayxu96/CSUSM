//===================================================================
// Assignment: Priority Queue of Processes
// Author: Runyu Xu
// Compiler: g++
// File type: ReadyQueue implementation file
// Date: 02/15/2018
//===================================================================
//Description: This file is the implementation file for the ReadyQueue
//             class,which will create a queue of processes that are in
//             the Ready state to be scheduled to run
//===================================================================
#include <iostream>
#include <vector>
#include "ReadyQueue.h"
#include "PCB.h"
using namespace std;

//Constructor
ReadyQueue::ReadyQueue()
{
	//do nothing
}

//Destructor
ReadyQueue::~ReadyQueue()
{
	//do nothing
}
//private methods
//get the left child of an element
//Parameter: parent -- the parent of the left child
int ReadyQueue::left(int parent)
{
	int left = 2 * parent + 1; // define the left child
	if (left > heap.size()) {
		return -1;
	}
	else {
		return left;
	}
}

//get the right child of an element
//Parameter: parent -- the parent of the right child
int ReadyQueue::right(int parent)
{
	int right = 2 * parent + 2; // define the right child
	if (right > heap.size()) {
		return 0;
	}
	else {
		return right;
	}
}

//get the parent of an element
//Parameter: child -- the child of an element(No matter left or right)
int ReadyQueue::parent(int child)
{
	int parent = (child - 1) / 2;
	if (child == 0) {
		return 0;
	}
	return parent;
}

//To move a node to higher level
//Parameter: index -- the index of a node (process)
void ReadyQueue::heapUp(int index)
{
	// if index is at 0 or out of bound, no need to up 
	if (index <= 0) {
		return;
	}
	int p = parent(index);
	//if parent's priority is lower than child, exchange
	if (heap[p].getPriority() > heap[index].getPriority())
	{
		PCB temp = heap[index];
		heap[index] = heap[p];
		heap[p] = temp;
		heapUp(p); //recursive do this until put the node in right position
	}
	if (heap[p].getPriority() == heap[index].getPriority() && heap[p].getID() > heap[index].getID()) {
		PCB temp = heap[index];
		heap[index] = heap[p];
		heap[p] = temp;
		heapUp(p); //recursive do this until put the node in right position
	}
}

//To move a node to lower level
//Parameter: index -- the index of a node (process)
void ReadyQueue::heapDown(int index)
{   //make sure the parent index is in the range of the heap
	if (index < 0 || index >= heap.size())
	{// parent is out of range
		return;
	}
	//define the left & right children and minmum child
	int leftChild = left(index);
	int rightChild = right(index);
	int minChild = index; // default set minChild to parent itself
	//make sure the leftChild and rightChild is in the range
	if (leftChild < 0) { // left child is out of range
		return;
	}
	if (rightChild == 0) {//right child is out of range
		return;
	}
	//confirm the smaller one of two children
	if (heap[leftChild].getPriority() > heap[rightChild].getPriority() ||
		(heap[leftChild].getPriority() == heap[rightChild].getPriority()&&
			heap[leftChild].getID() > heap[rightChild].getID()))
	{
		minChild = rightChild;
	}
	else {
		minChild = leftChild;
	}
	if (minChild != index) { //parent is not the leaf in the heap
		PCB temp = heap[index];
		heap[index] = heap[minChild];
		heap[minChild] = temp;
		heapDown(minChild);
	}
}

//Get the min value from root (first node in the queue)
PCB ReadyQueue::getMin()
{
	return heap[0];
}

//Remove the min element
void ReadyQueue::deleteMin()
{
	if (heap.empty()) { // if the heap is empty, do nothing
		return;
	}
	heap[0] = heap.back(); // move the first node to last
	heap.pop_back(); //remove the last node
	heapDown(0); // reheap the queue
}

//public methods
//Insert a process into the ready queue
//Parameter: process -- the process to add
void ReadyQueue::insertProc(PCB process)
{
	while (checkProc(process) == false) { //while the process is not in the list
		process.setState("Ready"); // change the state to "Ready"
		heap.push_back(process);// add process to the end of queue
		heapUp(heap.size() - 1);// up the process to correct position
	}
}

//Remove andreturn the process with highest priority(the first one)
PCB ReadyQueue::removeHighestProc() {
	PCB temp = getMin(); // set temp to the highest process
	deleteMin(); // remove the highest process
	temp.setState("Running");// change its state to "Running"
	return temp; // return this process
}
//Size of the ready queue
//Return the size of the queue
int ReadyQueue::size()
{
	return heap.size();
}

//Display all processes in the queue

void ReadyQueue::displayQueue()
{
	std::cout << std::left << "      " << "ID" << "      " << "Priority" << "      " << "State\n";
	std::cout << "===================================================\n";

	if (heap.empty()) {
		std::cout << "empty\n";
	}
	for (int i = 0; i < heap.size(); i++) {
		std::cout << std::left <<"      "<< heap[i].getID() << "      " << heap[i].getPriority() << "      "<< heap[i].getState() << "\n";
	}
	std::cout << "\n\n";
}

// reheap the queue
void ReadyQueue::reHeap()
{   // use for loop to reHeap all nodes in the heap
	for (int i = heap.size() - 1; i > -1; i--)
	{
		heapUp(i);
	}
}

bool ReadyQueue::checkProc(PCB process) {
	//check the process is already in the queue or not
	for (int i = 0; i < heap.size(); i++) {
		if (process.getID() == heap[i].getID()) {
			return true;
		}
	}
	return false;
}
