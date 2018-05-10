//============================================================
// CS 433 Operating System
// Assignment 3: CPU Scheduling
// Name: Runyu Xu
// Date: 03/28/2018
// File type: Simulation header file
// Description: General simulation class to simulater a CPU 
//============================================================
#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <queue>
#include <iostream>
#include "process.h"
#include "event.h"

using namespace std;

class Simulation {
protected:
	priority_queue<Event> eventQueue; // event Queue for DES
	vector<Process*> processList; // list of processes
	Process* CPU; // the process which is on CPU now
	int maxTime; // max runtime

public:
	Simulation(); // default constructor
	~Simulation(); // default destructor
	void scheduler(int num, int max); // a scheduler simulation
	virtual void initialProcesses(int num); // initial all processes for test
	virtual void processArrival(Event &event); // handle for process arrival
	virtual void cpuComplete(Event &event); // handle for cpu completion
	virtual void IOComplete(Event &event); // handle for I/O completion
	virtual void timerExpiration(Event &event); // handle for timer expiration
	virtual void handleEvent(Event &event); // handle for event
	void report(); // print the report of the simulation
	void clearProcesses(); // delete all processes
};
#endif 