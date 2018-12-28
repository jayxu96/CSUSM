#include<iostream>
#include<string>
using namespace std;

//-------------------------------------------------------
// CS421 HW1 
// Write a recognizer in C++ for L = {x | x is a binary number}.
// Your name: Runyu Xu
// Student ID: 005200674
//-------------------------------------------------------

//  The recognizer function should return TRUE or FALSE 
//  checking each character
//  to make sure it is 0 or 1. (leading 0's are allowed).
bool recognizer(string s)
{
	bool isBinary;// a boolean parameter used to check is a char is binary
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == '0' || s[i] == '1'){
			isBinary = true; // if the char is binary, set to true
		}
		else {
			isBinary = false; // otherwise, set to false and exit
			break;
		}
	}
	return isBinary;
}// end of recognizer

//main: Given a string (from E) cined from the user, pass it to
//      the recognizer function.
//      Cout YES IN L or NO NOT IN L depending on what was returned.
int main()
{
	string enter = " ";
	while (enter != "exit") {
		cout << "Please enter a string, enter 'exit' to stop: " << endl;
		cin >> enter;
		cout << "Your input is: " << enter << endl;
		bool isBinary = recognizer(enter);
		if (isBinary == true) {
			cout << "This is a binary number" << endl;
		}
		else if(enter != "exit"){
			cout << "This is not a binary number" << endl;
		}
	}

    // feel free to put it in a loop 

}// end of main
