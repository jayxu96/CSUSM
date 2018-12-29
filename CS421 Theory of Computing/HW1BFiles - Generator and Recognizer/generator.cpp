#include<iostream>
#include<string>
using namespace std;
#include "queue.h"  // include queue.h or equivalent such as <queue>,copy from CS311 HW1

//----------------------------------------------
// CS421 HW1 
// Write a generator in C++ for L = {x | x is a binary number}.
// Your name: Runyu Xu
//----------------------------------------------

// Copy the recognizer function here from the other file.
//  The recognizer function should return TRUE or FALSE 
//  checking each character
//  to make sure it is 0 or 1. (leading 0's are allowed).
bool recognizer(string s)
{
	bool isBinary;// a boolean parameter used to check is a char is binary
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == '0' || s[i] == '1') {
			isBinary = true; // if the char is binary, set to true
		}
		else {
			isBinary = false; // otherwise, set to false and exit
			break;
		}
	}
	return isBinary;
}// end of recognizer


// main: It should create each string over E = {0,1,2} systematically
//   (short to long) and pass each string to the recognizer function
//   created in Part a).  Hint: use a queue to generate strings
//   Only those strings for which the recognizer returned TRUE
//   should be displayed. 
//   Keeps on going until the queue overflows but the user can
//   terminate the program with control-C 
//   after about 20 strings have been displayed.
int main()
{
	string E[] = { "0", "1", "2" }; // the set of E
	string num;//the num to push into recognizer
	string newNum;// the new num created systematically
	queue myNumbers;// the queue to hold all the numbers
	//first initialize myNumbers
	for (int i = 0; i < sizeof(E)/sizeof(E[0]); i++) {
		myNumbers.add(E[i]);
	}
	cout << "The binary number generated by the set {0,1,2} are: " << endl;
	try {
		while (true) {
			myNumbers.remove(num);//remove current num from myNumbers
			if (recognizer(num)) { //push num into recognizer 
				cout << num << endl; // if return true, display it
			}
			if (!myNumbers.isFull()) { // create new num and add into myNumbers
				for (int i = 0; i < sizeof(E) / sizeof(E[0]); i++) {
					newNum = num + E[i];
					myNumbers.add(newNum);
				}
			}
		}// end of while
	}
	catch (queue::Underflow) { //catch any errors, and exit when we hit one.
		cerr << "[ERROR]: myNumbers is empty." << endl;
		return 0;
	}
	catch (queue::Overflow) {
		cerr << "[ERROR]: myNumbers is full." << endl;
		return 0;
	}
}// end of main