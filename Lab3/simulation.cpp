//============================================================
// CS 433 Operating System
// Assignment 3: CPU Scheduling
// Name: Runyu Xu
// Date: 03/28/2018
// File type: Simulation implementation file
// Description: General simulation class to simulater a CPU 
//============================================================
#include <iomanip>
#include <iostream>
#include "simulation.h"

Simulation::Simulation() {
	CPU = NULL;
}

Simulation::~Simulation() {
	//do nothing
}

void Simulation::scheduler(int num, int max) {
	initialProcesses(num);// initial all processes
	int currentTime = 0;
	maxTime = max;
	Event next;
	while ((!eventQueue.empty()) && (currentTime < maxTime)) {
		next = eventQueue.top();
		eventQueue.pop();
		currentTime = next.getStartTime();
		handleEvent(next);
	}
	report();
	clearProcesses();
}

// Purpose: initial all processes with random values
// Parameter: num -- the number of processes
void Simulation::initialProcesses(int num) {
	for (int i = 0; i < num; i++) {
		Process* newP = new Process(i,0); //create a new process
		processList.push_back(newP);// add to process list
		Event newEvent(ARRIVAL, newP->getStartTime(), i);// create an event of this process
		eventQueue.push(newEvent);// add this event to the event queue
	}
}

// Purpose: display the report of the CPU scheduling simulation
void Simulation::report() {
	int totalTurnaroundTime = 0; // total turn around time
	int completedJobs = 0; // number of completed jobs
	int cpuUtilization = 0; // cpu utilization
	int totalWaitingTime = 0; // total waiting time
	cout << "---------------------------------------------------------" << endl;
	cout << left << setw(5) << "ID" << setw(10) << "arrival" << setw(9) << "finish"
		<< setw(11) << "service" << setw(15) << "turnaround" << "wait" << endl;
	cout << "---------------------------------------------------------" << endl;
	// display the report of each process
	for (int i = 0; i < processList.size(); i++) {
		long completedTime = processList[i]->getCompletedTime(); // define the complete time of a process
		long startTime = processList[i]->getStartTime(); // define the start time of a process
		long serviceTime = processList[i]->getServiceTime(); //define the service time of a process
		long ioTime = processList[i]->getIOTime(); // define the I/O time of a process
		long turnaroundTime; // define the turnaround time of a process
		long waitingTime = maxTime; // worst case, the process didn't run
		if (completedTime > 0) {
			turnaroundTime = completedTime - startTime;
			waitingTime = turnaroundTime - serviceTime
				- ioTime;
			processList[i]->setWaitingTime(waitingTime);
			completedJobs++;
			totalTurnaroundTime += turnaroundTime;
		}
		else {
			turnaroundTime = 0;
			waitingTime -= startTime - serviceTime - ioTime;
			processList[i]->setWaitingTime(waitingTime);
		}
		totalWaitingTime += waitingTime;
		cout << left << setw(7) << processList[i]->getId() << setw(9) << processList[i]->getStartTime() / 1000
			<< setw(10) << processList[i]->getCompletedTime() / 1000 << setw(13) << processList[i]->getServiceTime() / 1000
			<< setw(12) << turnaroundTime / 1000 << processList[i]->getWaitingTime() / 1000 << endl;
		cpuUtilization += processList[i]->getServiceTime();
	}
	cout << "**Process times shown in seconds**" << endl << endl;
	cout << "Jobs Completed: " << completedJobs << endl;
	float utilizeCPU = (float(cpuUtilization) / float(maxTime)) * 100;
	cout << "CPU Utilization: " << fixed << setprecision(2) << utilizeCPU << " %" << endl;
	float throughput = (float(completedJobs) / float(processList.size())) / (float(maxTime) / 1000);
	cout << "Throughput: " << fixed << setprecision(3) << throughput << " jobs / sec" << endl;
	if (completedJobs != 0) {
		float avgTurnAround = float(totalTurnaroundTime) / float(completedJobs)
			/ (float(maxTime) / 1000);
		cout << "Avg Turnaround Time: " << fixed << setprecision(2) << avgTurnAround << " sec" << endl; 
	}
	else {
		cout << "Average turnaround time: NA" << endl;
	}
	float avgWaitTime = (float(totalWaitingTime) / float(processList.size()));
	cout << "Avg Wait Time: " << fixed << setprecision(2) << avgWaitTime / 1000 << " sec" << endl;
	cout << "---------------------------------------------------------" << endl << endl;
}


void Simulation::clearProcesses() {
	for (int i = 0; i < processList.size(); i++) {
		if (processList[i] != NULL) {
			delete processList[i];
			processList[i] = NULL;
		}
	}
}

void Simulation::handleEvent(Event &event) {
	int type = event.getType();
	switch (type) {
	case ARRIVAL:
		processArrival(event);
		break;
	case BURST_COMPLETION:
		cpuComplete(event);
		break;
	case IO_COMPLETION:
		IOComplete(event);
		break;
	case TIMER_EXPIRATION:
		timerExpiration(event);
		break;
	}
}

void Simulation::timerExpiration(Event &event)
{
	//Dummy Function
}
void Simulation::cpuComplete(Event &event)
{
	//Dummy Function
}
void Simulation::processArrival(Event &event)
{
	//Dummy Function
}
void Simulation::IOComplete(Event &event)
{
	//Dummy Function
}