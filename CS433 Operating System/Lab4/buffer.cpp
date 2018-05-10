//============================================================
// CS 433 Operating System
// Assignment 4: Producer - Consumer Problem
// Name: Runyu Xu
// Date: 04/08/2018
// File type: buffer implementation file
// Description: fixed bounded-buffer used in producer and consumer
//============================================================

#include "buffer.h"
using namespace std;

//variables
buffer_item buffer[BUFFER_SIZE]; // circular buffer

int count = 0; // counter used to count the number of items in the buffer
int front = 0;  // the first item's position of the buffer array initial with 0
int next = 0;  // the next item's position of the buffer array

// Purpose: check if the buffer is full
// Return : true if the count reach the BUFFER_SIZE, otherwise return false
bool isFull() {
	return (count == BUFFER_SIZE);
}

// Purpose: check if the buffer is empty
// Reuturn: true if the count is 0, otherwise return false
bool isEmpty() {
	return (count == 0);
}

// Purpose: insert an item into the circular buffer array
// Parameter: item -- the item to insert
// Return: 0 -- if insertion is sucessful
//         -1 -- otherwise return -1
int insert_item(buffer_item item) {
	// insert item into the buffer
	// if the buffer is not full, then add the item into the buffer
	if (!isFull()) {
		buffer[next] = item; // add the item at the end of hte buffer
		next = (next + 1) % BUFFER_SIZE; // update the next available position
		count++; // increase the number of items in the buffer
		return 0; 
	}
	return -1;
}

// Purpose: remove an item from the circular buffer array
// Parameter: *item -- to hold the item be removed from the buffer
// Return: 0 -- if remvoing is successful
//         -1 -- otherwise, return -1
int remove_item(buffer_item *item) {
	//remove an object from buffer
	if (!isEmpty()) { // if the buffer is not empty then remove the first item in the buffer
		*item = buffer[front]; // store the value of the removed item
		front = (front + 1) % BUFFER_SIZE; // update the first item in the buffer
		count--; // decrease the number of items in the buffer
		return 0;
	}
	return -1;
}

// Purpose: display the whole buffer in the entering order
void displayBuffer() {
	if (isEmpty()) {
		cout << "[ empty ] \n";
	}
	else {
		cout << "[ ";
		for (int i = front, j = 0; j < count; i = (i + 1) % BUFFER_SIZE, j++) {
			cout << buffer[i] << " ";
		}
		cout << "] \n";
	}

}