//============================================================
// CS 433 Operating System
// Assignment 3: CPU Scheduling
// Name: Runyu Xu
// Date: 03/28/2018
// File type: process implementation file
// Description: process class with several variables
//============================================================
#include "process.h"

// Purpose: constructor to create a new process with specify id and priority
Process::Process(int newId, int newPriority) {
	// give a new process with its initial values
	id = newId;
	priority = newPriority;

	randomStartTime(300000); // start time between 0-5min
	randomTotalCpuDuration(1000, 60000); // Total cpu duration between 1s - 1min
	randomAvgCpuBurstLength(5, 100); // avg cpu burst length between 5ms -100ms
	remainingCpuDuration = totalCpuDuration; // initial remain duration
	nextCpuBurstLength = 0; // initial with 0
	IOBurstTime = 0; // initial with 0
	status = READY; // set status to Ready

	//initial all result variables to 0
	completedTime = 0;
	serviceTime = 0;
	IOTime = 0;
	waitingTime = 0;
}

Process::~Process() {
	//do nothing
}

// Get methods for a process
int Process::getId() {
	return id;
}

long Process::getStartTime() {
	return startTime;
}

long Process::getTotalCpuDuration() {
	return totalCpuDuration;
}

long Process::getRemainingCpuDuration() {
	return remainingCpuDuration;
}

long Process::getAvgCpuBurstLength() {
	return avgCpuBurstLength;
}

long Process::getNextCpuBurstLength() {
	return nextCpuBurstLength;
}

long Process::getIOBurstTime() {
	return IOBurstTime;
}

int Process::getPriority() {
	return priority;
}

int Process::getStatus() {
	return status;
}

long Process::getCompletedTime() {
	return completedTime;
}

long Process::getServiceTime() {
	return serviceTime;
}

long Process::getIOTime() {
	return IOTime;
}

long Process::getWaitingTime() {
	return waitingTime;
}

// Set methods fo a process
// Purpose: add a new cpu duration of a process to total duration
void Process::setTotalCpuDuration(long duration) {
	totalCpuDuration += duration; 
}

// Purpose: after a process complete its CPU burst, reset
// the remaining CPU duration
void Process::setRemainingCpuDuration(long duration) {
	remainingCpuDuration = duration;
}

// Purpose: set the average CPU burst length randomly
void Process::setAvgBurstLength(long avg) {
	avgCpuBurstLength = avg;
}

// Purpose: set next CPU burst length 
void Process::setNextCpuBurstLength() {
	nextCpuBurstLength = CPUBurstRandom(avgCpuBurstLength);
}

// Purpose: set I/O burst time between 30 to 100 ms for a process
void Process::setIOBurstTime() {
	IOBurstTime = rand() % 100 + 30;
}

// Purpose: set the status of a process
void Process::setStatus(int newStatus) {
	status = newStatus;
}
	
// Purpose: set the completed time of a process
void Process::setCompletedTime(long complete) {
	completedTime = complete;
}

// Purpose: set the service time of a process
void Process::setServiceTime(long service) {
	serviceTime = service;
}

// Purpose: set the I/O time of a process
void Process::setIOTime(long IO) {
	IOTime = IO;
}

// Purpose: set waiting time of a process
void Process::setWaitingTime(long waiting) {
	waitingTime = waiting;
}


// Private methods
// Purpose: get a random start time during a 5 min time span for a process
// Parameter: time -- the tiem span
void Process::randomStartTime(long time) {
	startTime = rand() % time;
}

// Purpose: get a random total CPU duration between 1s to 1min
// Parameter: min -- the min time 
//            max -- the max time
void Process::randomTotalCpuDuration(long min, long max) {
	totalCpuDuration = rand() % max + min;
}

// Purpose: get a random avg CPU burst length between 5ms to 100ms
// Parameter: min -- the min time
//            max -- the max time
void Process::randomAvgCpuBurstLength(long min, long max) {
	avgCpuBurstLength = rand() % max + min;
}