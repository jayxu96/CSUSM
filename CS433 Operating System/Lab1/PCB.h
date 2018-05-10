//===================================================================
// Assignment: Priority Queue of Processes
// Author: Runyu Xu
// Compiler: g++
// File type: PCB header file
// Date: 02/14/2018
//===================================================================
//Description: This file is the header file for the PCB class, which
//             representing a process in the system.
//             For a process, it will have an ID, a priority and a 
//             state
//===================================================================

#ifndef PCB_H
#define PCB_H
#include <iostream>
#include <fstream>
#include<string>
using namespace std;

class PCB {
private:
	int pcbID;    // ID number of a process
	int pcbPriority;    // Priority value of a process
	string state;   // Current pcb state of a process

public:
	PCB();    // Default constructor
	PCB(int id, int priority, string s); // constructor of PCB
	~PCB();    // Destructor

	int getID();    // Return process ID
	void setID(int id);   // Set process ID  
	int getPriority();    // Return process priority
	void setPriority(int priority);   // Set process priority
	string getState();    // Return process state
	void setState(string s);    // Set process state
};
#endif //PCB_H