//============================================================
// CS 433 Operating System
// Assignment 3: CPU Scheduling
// Name: Runyu Xu
// Date: 03/28/2018
// File type: SJF implementation file
// Description: a simulation with SJF algorithm
//============================================================
#include "SJF.h"

// Default constructor
SJF::SJF() {
	CPU = NULL;// initial CPU is idle (hold nothing)
}

// Default destructor
SJF::~SJF() {
	// do nothing
}

// Purpose: handle a process arrival event
void SJF::processArrival(Event &event) {
	readyQueue.push(processList[event.getID()]);
	dispatch(event);
}

// Purpose: handle a CPU completion event
void SJF::cpuComplete(Event &event) {
	Process* currentProcess = CPU; // set the process in CPU to current process
	CPU = NULL; // free CPU
	currentProcess->setRemainingCpuDuration(currentProcess->getRemainingCpuDuration()
		- currentProcess->getNextCpuBurstLength()); // update the remaining CPU duration

	// if the process is completed
	if (currentProcess->getRemainingCpuDuration() < 1) {
		currentProcess->setStatus(TERMINATED); // change status to terminated 
		currentProcess->setCompletedTime(event.getStartTime()); //set the finish time of a process
	}
	else { // else the process is not completed
		currentProcess->setIOBurstTime(); // change to I/O
		currentProcess->setStatus(WAITING); // change status to waiting
        //create a new IO event and add it to event queue
		long IOStartTime = event.getStartTime() + currentProcess->getIOBurstTime();
		Event newIOEvent(IO_COMPLETION, IOStartTime, currentProcess->getId());
		eventQueue.push(newIOEvent);
	}
	dispatch(event);
}

// Purpose: handle a I/O completion event
void SJF::IOComplete(Event &event) {
	Process* nextProcess = processList[event.getID()]; // get the next process to add to ready queue
	nextProcess->setStatus(READY); // change status to ready
	nextProcess->setIOTime(nextProcess->getIOTime() + nextProcess->getIOBurstTime()); //set next IO time
	readyQueue.push(nextProcess); // add next process to ready queue
	dispatch(event);
}

// Purpose: handle a timeer expiration event
void SJF::timerExpiration(Event &event) {
	Process* process = processList[event.getID()];
	process->setStatus(READY);
	process->setNextCpuBurstLength(); //set next CPU burst length
	readyQueue.push(process);
	dispatch(event);
}

// Purpose: dispatch a process through event object
void SJF::dispatch(Event &event) {
	if ((CPU == NULL) && (!readyQueue.empty())) {
		Process* nextProcess = readyQueue.top(); // get the fisrt process in the ready queue
		readyQueue.pop(); // remove it from the ready queue
		CPU = nextProcess; // add this process to CPU
		CPU->setNextCpuBurstLength(); // set next CPU burst length
		long eventStartTime; // define the start time of a new event
		// if next CPU burst length is bigger than remaining duration of a process
		if (CPU->getNextCpuBurstLength() > CPU->getRemainingCpuDuration()) {
			eventStartTime = CPU->getRemainingCpuDuration()
				+ event.getStartTime(); // get the start time of the new event
			nextProcess->setServiceTime(nextProcess->getServiceTime()
				+ nextProcess->getRemainingCpuDuration()); // update the service time of this process
		}
		else { // else update data by adding the next CPU burst length
			eventStartTime = CPU->getNextCpuBurstLength()
				+ event.getStartTime();
			nextProcess->setServiceTime(nextProcess->getServiceTime()
				+ nextProcess->getNextCpuBurstLength());
		}
		Event newEvent(BURST_COMPLETION, eventStartTime, CPU->getId());
		eventQueue.push(newEvent);
	}
}