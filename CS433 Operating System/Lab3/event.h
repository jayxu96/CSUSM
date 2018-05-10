//============================================================
// CS 433 Operating System
// Assignment 3: CPU Scheduling
// Name: Runyu Xu
// Date: 03/14/2018
// File type: event header file
// Description: event class which hold a process
//============================================================
#ifndef EVENT_H
#define EVENT_H

// Define the types of event
enum {
	ARRIVAL,
	BURST_COMPLETION,
	IO_COMPLETION,
	TIMER_EXPIRATION
};

class Event{
// Private parameters
private:
	int type; // type of event
	long startTime; // start time of a event
	int id; // the id of a process which is in the event

// Public methods
public: 
	Event(); // default constructor
	// constructor to create a new event
	Event(int newType, long time, int newId); 
	~Event(); // default destructor

	// Get methods
	long getStartTime();// return the start time of an event
	int getType(); //return the type of an event
	int getID(); // return the id of the process in the event

	bool operator< (const Event &param) const;
};
#endif