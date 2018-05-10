//============================================================
// CS 433 Operating System
// Assignment 3: CPU Scheduling
// Name: Runyu Xu
// Date: 03/28/2018
// File type: SJF simulation header file
// Description: a simulation with SJF algorithm
//============================================================
#ifndef SJF_H
#define SJF_H

#include "simulation.h"

class SJF : public Simulation
{
public:
	SJF(); // default constructor
    ~SJF(); // default destructor

	virtual void processArrival(Event &event); // handle for process arrival
	virtual void cpuComplete(Event &event); // handle for cpu completion
	virtual void IOComplete(Event &event); // handle for I/O completion
	virtual void timerExpiration(Event &event); // handle for timer expiration
	virtual void dispatch(Event &event); //dispatch an event

protected:
	//priority_queue<Process*> readyQueue;
	priority_queue<Process*, vector<Process*>, Process::CompStr > readyQueue;
};
#endif 