//===================================================================
// Assignment: Priority Queue of Processes
// Author: Runyu Xu
// Compiler: g++
// File type: Driver file
// Date: 02/14/2018
//===================================================================
//Description: This file is to test two parts of test
//===================================================================

#include <iostream>
#include <vector>
#include <sys/time.h>
#include <cstdlib>
#include "ReadyQueue.h"
#include "PCB.h"
#include "PCB_Table.h"
using namespace std;


// test 1 of this assignment
void test1(ReadyQueue &q1, PCB_Table &t1)
{
	cout << "Test 1 \n";
	cout << "Step a: add process 5, 1, 8, 11 to the ready queue and display them\n";
	//(a) add process 5, 1, 8, 11 to the queue and display them
	q1.insertProc(t1.getProcess(5));
	q1.insertProc(t1.getProcess(1));
	q1.insertProc(t1.getProcess(8));
	q1.insertProc(t1.getProcess(11));
	q1.displayQueue();


	//(b) remove the process with the highest priority from q1 and display q1.
	cout << "Step b: remove the process with the highest priority\n";
	q1.removeHighestProc();
	q1.displayQueue();

	//(c)remove the process with the highest priority from q1 and display q1
	cout << "Step c: remove the process with the highest priority\n";
	q1.removeHighestProc();
	q1.displayQueue();

	//(d) Insert processes 3, 7, 2, 12, 9 to q1
	cout << "Step d: insert process 3, 7, 2, 12, 9 to the ready queue\n";
	q1.insertProc(t1.getProcess(3));
	q1.insertProc(t1.getProcess(7));
	q1.insertProc(t1.getProcess(2));
	q1.insertProc(t1.getProcess(12));
	q1.insertProc(t1.getProcess(9));
	q1.displayQueue();

	// (e) One by one remove the process with the highest priority from the queue q1 and display it after each removal.
	cout << "Step e: one by one remove the process \n";
	while (q1.size() != 0) {
		q1.removeHighestProc();
		cout << "After remove the process with the highest priority:\n";
		q1.displayQueue();
		cout << "\n";
	}
}
//test2 of the assignment
void test2(ReadyQueue &q2, PCB_Table &t2) {
	cout << "Test 2\n";
	srand(time(0));
	//randomly select 10 processes with random priority from the table
	for (int i = 1; i < 11; i++) {
		int randIndex = rand() % 19;
		PCB randProc;
		randProc = t2.getProcess(randIndex);
		randProc.setPriority(rand() % 50 + 1);// priority between 1-50
		q2.insertProc(randProc);
		if (q2.size() < i) {
			i -= 1;
		}
	}
	q2.displayQueue();

	//start of test 2
	timeval endTime;
	timeval startTime;
	gettimeofday(&startTime, NULL);
	//repeat 100000 times to add or remove a process
	int selection; // the selection between add or remove a process
	int randIndex; // the random index
	PCB proc; // the proc to add
	for (int i = 0; i < 1000000; i++) {
		selection = rand() % 2; // set selection with 0 or 1
		if (selection == 0 && q2.size() != 0) {//if the selection is 0 and the queue is not empty 
			q2.removeHighestProc(); // remove the highest priority process
		}
		else {
			randIndex = rand() % 19;
			proc = t2.getProcess(randIndex);
			proc.setPriority(rand() % 50 + 1);//set priority between 1-50
			q2.insertProc(proc);// add the process
			if (q2.checkProc(proc)) {// check if the process is already in the queue
				i -= 1; // decrease the value of i to try again
			}
		}
	}
	gettimeofday(&endTime, NULL);
	// Convert the times to seconds
	double initialTime = startTime.tv_sec + (startTime.tv_usec / 1000000.0);
	double finalTime = endTime.tv_sec + (endTime.tv_usec / 1000000.0);
	cout << finalTime - initialTime << " seconds elapsed to process "
		<< "1000000 cycles." << endl;
	q2.reHeap();
	q2.displayQueue();

}

int main()
{
	//print out the header
	cout << "=========================================" << endl;
	cout << "Author: Runyu Xu" << endl;
	cout << "Course: CS433 Operating Systems" << endl;
	cout << "Assignment: Priority Queue of Processes" << endl;
	cout << "=========================================" << endl;

	//Create a PCB Table with size 20
	PCB_Table table;
	table.makeTable(20);
	ReadyQueue queue; // create an empty queue for test
	test1(queue, table);
	test2(queue, table);
}

