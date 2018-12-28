//INSTRUCTION
//Look for **
//Fill in the function body
//When done, compile queue.cpp to make sure it has no syntax errors

//=========================================================
//HW#: HW1P1 queue
//Your name: Runyu Xu
//Complier:  g++
//File type: Queue implementation file
//===========================================================

using namespace std;
#include <iostream>
#include "queue.h"  

// constructor to create an object
queue::queue()
{
	count = 0; // empty queue
	front = 0; // the start index is 0
	rear = -1; // rear index to -1 
}

//destructor to destroy an object
queue::~queue()
{
}

// PURPOSE: returns true if queue is empty, otherwise false
bool queue::isEmpty()
{
	if (count == 0) {    //Check to see if number of elements is 0
		return true;
	}
	return false; //default the queue is not empty
}

// PURPOSE: returns true if queue is full, otherwise false
bool queue::isFull()
{
	if (count == MAX_SIZE) {  //Check if count reach the MAX_SIZE
		return true;
	}
	return false; //default the queue is not full
}

// PURPOSE: if full, throws an exception Overflow
// if not full, enters an element at the rear 
// PAREMETER: provide the element (newElem) to be added
void queue::add(el_t newElem)
{
	if (isFull()) {     //check if the queue is already full
		throw Overflow();
	}
	else {
		rear = (rear + 1) % MAX_SIZE; // set the new rear index
		el[rear] = newElem; // add new element to rear
		count++; // increase count
	}
}

// PURPOSE: if empty, throw Underflow
// if not empty, removes the front element to give it back 
// PARAMETER: provide a holder (removedElem) for the element removed (pass by ref)
void queue::remove(el_t& removedElem)
{
	if (isEmpty()) {    //check if the queue is empty
		throw Underflow();
	}
	else {
		removedElem = el[front];     //define removedElem to the element on front
		front = (front + 1) % MAX_SIZE;   //set a new front index
		count--;    //reduce the count
	}
}

// PURPOSE: if empty, throws an exception Underflow
// if not empty, give back the front element (but does not remove it)
//PARAMETER: provide a holder (theElem) for the element (pass by ref)
void queue::frontElem(el_t& theElem)
{
	if (isEmpty())      //check if the queue is empty 
		throw Underflow();
	else {
		theElem = el[front]; 
	}
}

// PURPOSE: returns the current size to make it 
// accessible to the client caller
int queue::getSize()
{
	return count;
}

// PURPOSE: display everything in the queue horizontally from front to rear
// enclosed in []
// if empty, displays [ empty ]
void queue::displayAll()
{
	if (!isEmpty()) {   // if the queue is not empty do following
		// read the queue from front to rear
		// since the queue is circular, rear can be before front, 
		// so we need use i % MAX_SIZE to wrap around
		for (int i = front; i < front + count; i++) {
			cout << "[ " << el[i % MAX_SIZE] << " ]";
		 }
		cout << endl;
	}
	else {  //the queue is empty, do following
		cout << "[ empty ]" << endl;
	}
}

// PURPOSE: if empty, throws an exception Underflow
//if queue has just 1 element, does nothing
//if queue has more than 1 element, moves the front one to the rear by
//doing remove followed by add.
void queue::goToBack()
{
	if (isEmpty())        //check if the queue is empty
		throw Underflow();
	if (count > 1) { //if queue has more than 1 element
		el_t temp;  // define a temp variable
		remove(temp);   //remove the last element and store it in temp
		add(temp);  //add temp back to the back of the queue
	}
}

