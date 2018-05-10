//============================================================
// CS 433 Operating System
// Assignment 3: CPU Scheduling
// Name: Runyu Xu
// Date: 03/28/2018
// File type: driver file
// Description: main to run these CPU schedules
//============================================================

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <cstdlib>
#include <cstring>
#include "FCFS.h"
#include "SJF.h"
using namespace std;

static const int MAX_RUNTIME = 300000; // 5 minutes in milliseconds
int main() {
	int num;
	int selection;
	cout << "---------------------------------------------------------" <<  endl;
	cout << "// Name: Runyu Xu" << endl;
	cout << "// Date : March 27, 2017" << endl;
	cout << "// Course : CS433 - Operating Systems" << endl;
	cout << "// Assignment : 3 - CPU scheduling" << endl;
	cout << "---------------------------------------------------------" << endl << endl;
	cout << "Enter number of processes to add into DES Simulation: ";
	cin >> num;
	cout << endl;
	cout << "Select scheduler type" << endl;
	cout << "1: FCFS" << endl;
	cout << "2: SJF" << endl << endl;

	cout << ">> ";
	cin >> selection;
	if (selection == 1) {
		FCFS newFCFS;
		newFCFS.scheduler(num, MAX_RUNTIME);
	}
	else if (selection == 2) {
		SJF newSJF;
		newSJF.scheduler(num, MAX_RUNTIME);
	}
}