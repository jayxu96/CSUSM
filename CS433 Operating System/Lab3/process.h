//============================================================
// CS 433 Operating System
// Assignment 3: CPU Scheduling
// Name: Runyu Xu
// Date: 03/28/2018
// File type: process header file
// Description: process class with several variables
//============================================================
#ifndef PROCESS_H
#define PROCESS_H

#include <cstdlib>
#include <ctime>
#include "random.h"

//define the status of a process
enum {
	READY, RUNNING, WAITING, TERMINATED
};


class Process {
private:  //define parameters of a process
	int id; // the id of a process
	long startTime; // the start time for process(will be randomly)
	long totalCpuDuration; // the total CPU duration of a process (randomly between 1s to 1min)
	long remainingCpuDuration; // the remained CPU duration
	int nextCpuBurstLength; // next CPU burst length
	long avgCpuBurstLength; // average CPU burst length (randomly between 5ms to 100ms)
	long IOBurstTime; // I/O burst tiem will random between 30 to 100 ms
	int priority; // the process priority based on scheduling
	int status; // the status of a process

	//parameters after executing a process
	long completedTime; // time for completed a process
	long serviceTime; // the time of a process using the CPU
	long IOTime; // the I/O time of a process
	long waitingTime; // the watiting time of a process

	// Purpose: get a random start time during a 5 min time span for a process
	// Parameter: time -- the tiem span
	void randomStartTime(long time);

	// Purpose: get a random total CPU duration between 1s to 1min
	// Parameter: min -- the min time 
	//            max -- the max time
	void randomTotalCpuDuration(long min, long max);

	// Purpose: get a random avg CPU burst length between 5ms to 100ms
	// Parameter: min -- the min time
	//            max -- the max time
	void randomAvgCpuBurstLength(long min, long max);
public:
	// Constructor for a process
	// Parameter: newId -- the id of a process
	//            newPriority -- the priority of a process
	Process(int newId, int newPriority);
	~Process(); // destructor

	//get methods to get the value of parameters of a process
	int getId();
	long getStartTime();
	long getTotalCpuDuration();
	long getRemainingCpuDuration();
	long getAvgCpuBurstLength();
	long getNextCpuBurstLength();
	long getIOBurstTime();
	int getPriority();
	int getStatus();

	//get methods for result parameters
	long getCompletedTime();
	long getServiceTime();
	long getIOTime();
	long getWaitingTime();

	//set methods for a process
	void setTotalCpuDuration(long duration);
	void setRemainingCpuDuration(long duration);
	void setAvgBurstLength(long avg);
	void setIOBurstTime();
	void setStatus(int newStatus);

	void setCompletedTime(long complete);
	void setServiceTime(long service);
	void setIOTime(long IO);
	void setWaitingTime(long waiting);

	void setNextCpuBurstLength();

	struct CompStr {
		bool operator()(Process* p1, Process* p2) {
			return p1->remainingCpuDuration > p2->remainingCpuDuration;
		}
	};
};

#endif /*end of PROCESS_H*/