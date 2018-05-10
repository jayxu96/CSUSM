//===================================================================
// Assignment: Priority Queue of Processes
// Author: Runyu Xu
// Compiler: g++
// File type: PCB_Table header file
// Date: 02/14/2018
//===================================================================
//Description: This file is the header file for the PCB_Table class,
//             which will create a table of processes
//===================================================================

#pragma once
#include "PCB.h"
#include <vector>
using namespace std;

class PCB_Table {
private:
	vector<PCB> table; // the list hold processes
public:
	//Constructor
	PCB_Table();
	//Destructor
	~PCB_Table();

	void makeTable(int n); // method to create a PCB Table with size n
	void display(); // display the contents of the table
	PCB& getProcess(int index); // return the process at position index

};