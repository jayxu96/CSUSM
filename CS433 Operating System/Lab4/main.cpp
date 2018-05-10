//============================================================
// CS 433 Operating System
// Assignment 4: Producer - Consumer Problem
// Name: Runyu Xu
// Date: 04/08/2018
// File type: driver file
// Description: main file to simulate the producer-consumber problem
//============================================================
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h> 
#include <time.h> 
#include <unistd.h>
#include "buffer.h"

using namespace std;

pthread_mutex_t mutex; // the mutex lock
sem_t full;  // semaphore of full
sem_t empty; // semaphore of empty
int sleeptime;
int insert_item(buffer_item item);
int remove_item(buffer_item *item);
void displayBuffer();


// Purpose: Producer to produce an item in the buffer
// Parameter: *param -- used to pass pthread_create function
void* producer(void *param) {
	buffer_item item;

	while (true) { 
		sleep(rand() % sleeptime + 1);   // sleep for a random period of time between 1 - sleeptime
		item = rand() % 100 + 1; // generate a random number between 1 - 100 // to keep the item not to big

		sem_wait(&empty);            // wait for the empty signal
		pthread_mutex_lock(&mutex);  // lock the mutex
		//access critical section
		if (!insert_item(item)) { //if item is added into the buffer successfully
			cout << "Item " << item << " inserted by a producer. The current content of the "
				<< "buffer is ";
			displayBuffer();
		}
		else {
			cout << "Error: Failed to add this item.\n";
		}

		//release the lock
		pthread_mutex_unlock(&mutex); // unlock the mutex
		sem_post(&full);  // give the full signal 
	}
}

// Purpose: Consumer to consume an item from the buffer
// Parameter: *param -- used to pass pthread_create function
void* consumer(void *param) {
	buffer_item item;
	while (true) {
		sleep(rand() % sleeptime + 1);   // sleep for a random period of time between 1- sleeptime
		sem_wait(&full); // wait for the full signal
		pthread_mutex_lock(&mutex); // lock the mutex
		//access critical section
		if (!remove_item(&item)) { // if item is removed successfully
			cout << "Item " << item << " removed by a consumer. The current content of the "
				<< "buffer is ";
			displayBuffer();
		}
		else {
			cout << "Error: Failed to remove this item.\n";
		}
		//release the lock 
		pthread_mutex_unlock(&mutex); // unlock the mutex
		sem_post(&empty); // give the empty signal
	}
}

// Purpose: Main to simulate the producer - consumer problem
int main(int argc, char *argv[])
{
	// input handling
	if (argc < 4 || argc > 4) {
		cout << "Usage: ./test   <sleeptime> <number of producer threads> <number of consumer threads>" << endl;
		exit(1);
	}

	srand(time(NULL));
	cout << "---------------------------------------------------------" << endl;
	cout << "// Name: Runyu Xu" << endl;
	cout << "// Date : April 8th, 2018" << endl;
	cout << "// Course : CS433 - Operating Systems" << endl;
	cout << "// Assignment : 4 - Producer-Consumber problem" << endl;
	cout << "---------------------------------------------------------" << endl << endl;
	// 1. Get command line arguments argv[1], argv[2], argv[3]
	sleeptime = atoi(argv[1]);		    // 1st arg is the sleep time of the main thread
	int pThreads = atoi(argv[2]);		// 2nd arg is the number of producer threads
	int cThreads = atoi(argv[3]);		// 3rd arg is the number of consumer threads

    // 2. Initialize buffer 
	pthread_t producer_threads[pThreads];
	pthread_t consumer_threads[cThreads];
	pthread_mutex_init(&mutex, NULL);
	sem_init(&empty, 0, BUFFER_SIZE);	// empty initialized to buffer size
	sem_init(&full, 0, 0);				// full initialized to 0

	// 3. Create producer thread(s)
	for (int i = 0; i < pThreads; i++) {
		pthread_create(&producer_threads[i], NULL, producer, NULL);
	}

	// 4. Create consumer thread(s)
	for (int i = 0; i < cThreads; i++) {
		pthread_create(&consumer_threads[i], NULL, consumer, NULL);
	}

	// 5. Sleep 
	sleep(sleeptime);

	// 6. Exit 
	exit(0);
	
}