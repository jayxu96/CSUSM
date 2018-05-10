//===================================================================
// Assignment: Priority Queue of Processes
// Author: Runyu Xu
// Compiler: g++
// File type: PCB_Table implementation file
// Date: 02/14/2018
//===================================================================
//Description: This file is the implementation file for the PCB_Table class,
//             which will create a table of processes
//===================================================================

#include <iostream>
#include "PCB_Table.h"
using namespace std;

//Default constructor 
PCB_Table::PCB_Table()
{
	//do nothing
}

//Default
PCB_Table::~PCB_Table()
{
	//do nothing
}

//Create a pcb table with specific amount of elements
//Parameter: n - the number of elements in the table
void PCB_Table::makeTable(int n)
{
	for (int i = 1; i <= n; i++) {// use for loop to create and add process
		PCB newPCB(i, i, "New");
		table.push_back(newPCB);
	}
}

//Return a pcb at a specific index
//Parameter: index - the index of a pcb
PCB&PCB_Table::getProcess(int index)
{
		return table.at(index - 1);
}

//Display all processes in the table
void PCB_Table::display() 
{
	std::cout << std::left << "      " << "ID" << "      " << "Priority" <<"      "<< "State" << std::endl;
	std::cout << "===================================================" << endl;

	for (int i = 0; i < table.size(); i++) {
		std::cout << std::left << "      "<< table[i].getID() << "      " << table[i].getPriority() << "      " << table[i].getState() << std::endl;
	}
}

