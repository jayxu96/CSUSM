//===================================================================
// Assignment: Priority Queue of Processes
// Author: Runyu Xu
// Compiler: g++
// File type: PCB implementation file
// Date: 02/14/2018
//===================================================================
//Description: This file is the implementation file for the PCB class,
//             which representing a process in the system.
//===================================================================
#include <iostream>
#include "PCB.h"
using namespace std;


//Default constructor
PCB::PCB()
{
	pcbID = 0;   // default ID is 0
	pcbPriority = 0; // default priority is 0
	state = "New";   // default state is "New"
}

//Constructor to craete a new process
//Parameter: id - the ID of a process
//           priority - the priority of a process
//           s - the state of a process
PCB::PCB(int id, int priority, string s) {
	this->pcbID = id;
	this->pcbPriority = priority;
	this->state = s;
}

//Destructor
PCB::~PCB() {
	//Do nothing
}

//Getters and setters

//Getter the ID of a process
// return the pcbID;
int PCB::getID() {
	return pcbID;
}

//set ID to a process
//Parmeter: id - the ID of a process
void PCB::setID(int id) {
	pcbID = id;
}

//Get the priority of a process
// return the priority
int PCB::getPriority() {
	return pcbPriority;
}

//Set the priority of a process
//Parmeter: priority - the priority of a process
void PCB::setPriority(int priority) {
	pcbPriority = priority;
}

//Get the state of a process
//return the state of a process
string PCB::getState() {
	return state;
}
//Set the state of a process
//Parmeter: s - the state of a process
void PCB::setState(string s) {
	state = s;
}