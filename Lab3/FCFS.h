//============================================================
// CS 433 Operating System
// Assignment 3: CPU Scheduling
// Name: Runyu Xu
// Date: 03/28/2018
// File type: FCFS simulation header file
// Description: a simulation with FCFS algorithm
//============================================================
#ifndef FCFS_H
#define FCFS_H
#include "simulation.h"

class FCFS : public Simulation
{
protected:
	queue<Process*> readyQueue; // ready queue for FCFS
public:
	FCFS(); //default constructor
	~FCFS(); //default destructor
	virtual void processArrival(Event &event); // handle for process arrival
	virtual void cpuComplete(Event &event); // handle for cpu completion
	virtual void IOComplete(Event &event); // handle for I/O completion
	virtual void timerExpiration(Event &event); // handle for timer expiration
	virtual void dispatch(Event &event); //dispatch an event
};
#endif