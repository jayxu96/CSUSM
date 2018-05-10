// ============================================
// Name: Runyu Xu
// Assignment: 2 - UNIX Shell and History Feature
// Compiler: g++
// File type: Driver file
// date : 03/05/2018
// Run program: can directly run on empress using ./a.out
// ============================================
// Note: At first, I tried to create my own vector
// struct, but there were some errors in it and I 
// cannot figure it out. So I changed to use array
// to hold history buffer
//


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>


#define MAX_LINE 80 /* The maximum length command */
#define MAX_SIZE 10 /*The maximum size of the history buffer*/
#define MAX_NUM_PARAM 10 /*The maximum number of parameters in a line*/

int count = 0; /*The count of history buffer*/
int elemIndex; /*The index of an element*/
char *historyBuffer[MAX_SIZE]; /*The history buffer to hold commands*/

//Purpose: record a command into the history buffer
//Parameter: command -- the command to be recorded
void addToHistory(char *command) {
	if (count < MAX_SIZE) { // add a command into the history buffer
		historyBuffer[count] = strdup(command);
		count++; // increase the count	
	}
	else {
		char *temp; // pointer to hold a command
		for (int i = 1; i < MAX_SIZE; i++) {
			temp = historyBuffer[i];
			historyBuffer[i - 1] = temp;
		}
		historyBuffer[count - 1] = strdup(command); // add command to end
	}
	elemIndex = count; // set the index of an element
}

//Purpose: display last ten commands
//         if no command there , display "No commands in history"
void displayHistory() {
	int index = count - 1; // the index of an element
	int num = 0; // the number of the commands to display
	if (count == 0) { // if the history buffer is empty, do following
		printf("Warning! No commands in history\n");
	}
	//while the buffer is not empty, display last ten elements
	while (num < 10 && index >= 0) {
		printf("%i - ", index + 1); // display the No. of the element
		printf(" %s", historyBuffer[index]);//the element
		printf("\n");
		index--; // go to next element
		num++; // 
	}
}

//Purpose: get the index of the most recent command
int getLastCommand() {
	return elemIndex;
}

//Purpose: parse a command line into 1 or more commands
//Parameter: command -- the command line entered
//           params -- the array to hold commands
// I supposed to use sizeof method instead of setting a maximum 
// numbers of parameters, but failed
void parseLine(char *command, char **params) {

	for (int i = 0; i < MAX_NUM_PARAM; i++) {
		params[i] = strsep(&command, " ");
		if (params[i] == NULL) {
			break;
		}
	}
}


//Purpose:This is a simple shell program include part1 create a child process 
//        and part2 display history feature
//1.when the enter command is a user command, it will create a new child process
// and run it, parent will wait until child is end unless an ampersand is entered
//2. display history will show the last 10 commands and !! will show the last command
// !+Num will show the Nth command
int main(void)
{
	char *args[MAX_LINE / 2 + 1]; /* command line arguments */
	int should_run = 1; /* flag to determine when to exit program*/
	printf("====================================================\n");
	printf("Authors      : Runyu Xu\n");
	printf("Course       : CS433 - Operating Systems\n");
	printf("Assignment2  : Unix Shell and History Feature\n");
	printf("Date         : 03/05/2018\n");
	printf("====================================================\n");

	while (should_run) {
		printf("osh>");
		fflush(stdout);

		//read input commands
		char command[100]; // store commands entered by user
		scanf("%s", command); // read the input commands
		parseLine(command, args);// parse the commands line to several commands

		addToHistory(*args);

		// Part2, display history
		char history[8] = "history";
		if (strcmp(command, history) == 0) {
			displayHistory();
			continue;
		}
		//===========================================================
		//Part1 - creating a child process
		/**
		 * After reading user input, the steps are:
		 * (1) fork a child process using fork()
		 * (2) the child process will invoke execvp()
		 * (3) if command didn't included &, parent will invoke wait()
		 */

		 //fork a child process
		pid_t pid;
		pid = fork();
		//case1 throw an error
		if (pid < 0) {
			fprintf(stderr, "Fork Failed\n");
			return 1;
		}
		//case2 fork a child process to execute user's commands
		else if (pid == 0) {
			//1. if enter is "!!"
			char recent[3] = "!!";
			if (strcmp(command, recent) == 0) {
				int last = getLastCommand();
				if (last <= 1) {
					printf("No commands in history\n");
					continue;
				}
				last = last - 1;
				printf("Most recent command is: %s", historyBuffer[last - 1]);
				printf("\n");
				execvp(historyBuffer[last - 1], args);
				continue;
			}
			//2. if enter is !+Num
			if (command[0] == '!') {
				int num;
				int one = 1;
				if (isdigit(command[1]))
				{
					//Get the number to execute and change to int
					num = atoi(&command[1]);

					//User command exceeds historyBuffer size
					if (num > count - 1)
					{
						printf("No such command in history.\n");
					}
					else
					{
						//Execute Nth command
						printf("Command %i is: %s", num, historyBuffer[num - 1]);
						printf("\n");
						execvp(historyBuffer[num - 1], args);
					}
				}
				continue;
			}
			//Check commands entered in is exit or not
			char end[5] = "exit";
			if (strcmp(command, end) == 0) {
				printf("Program Terminated\n");
				should_run = 0;
				exit(0);
			}
			else {
				//Execute command
				execvp(args[0], args);
				//Incorrect command was entered
				int executeReturn = execvp(args[0], args);
				if (executeReturn == -1)
				{
					exit(0);
				}
			}
		}
		else // parent process
		{
			if (!(command == "&"))
			{
				wait(NULL);
			}
			else
			{
				exit(0);
			}
		}
	}
	return 0;
}