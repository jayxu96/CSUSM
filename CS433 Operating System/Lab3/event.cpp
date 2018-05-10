//============================================================
// CS 433 Operating System
// Assignment 3: CPU Scheduling
// Name: Runyu Xu
// Date: 03/14/2018
// File type: event implementation file
// Description: event class which hold a process
//============================================================
#include "event.h"

// Default constructor
Event::Event() {
	id = 0;
	type = ARRIVAL;
	startTime = 0;
}

// Constructor to create a new event
Event::Event(int newType, long time, int newId) {
	id = newId;
	type = newType;
	startTime = time;
}

// default destrucctor
Event::~Event() {
	// do nothing
}

// Purpose: get the id of the process in the event
int Event::getID() {
	return id;
}

// Purpose: get the type of an event
int Event::getType() {
	return type;
}

// Purpose: get the start time of an event
long Event::getStartTime() {
	return startTime;
}

bool Event::operator<(const Event &param) const
{
	// Implemented using > so that priority is reversed
	return (startTime > param.startTime);
}