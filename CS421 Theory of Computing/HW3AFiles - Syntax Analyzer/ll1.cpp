#include<iostream>
#include<stdlib.h>
#include<fstream>
//#include"stack.h"  // ** can be removed
#include<vector>
#include<string>
using namespace std;

//---------------------------------------
// CS421 File ll1.cpp for HW3A
// Your name: Runyu Xu
//---------------------------------------

// ** Complete this to fit the HW3A specification - look for **
// ** Must have the same types of tracing couts as my demo program.
// ** Feel free to use your stack.h and stack.cpp or use vectors

// Global variables
vector<char> M[3][5];  // the table of rules
vector<char> stack; // global stack to store current rules

// Method to read rules into matrix Machine
void readrules()
{
	ifstream fin("rules", ios::in);
	// ** read in the rules into M, make sure $ is not stored

	char nt, x;   // nt is the char for non-terminal, x for the first char after nt
	int row = 0;  // row of rule in matrix
	int col = 0;  // column of rule in matrix
	int charpos = 1; // char position in line
	string line;     // read the file line by line
	vector<char> temp; // temp vector to hold the rule

	while (getline(fin, line)) { // read the file line by line

		//determine the row of a rule
		nt = line[0]; // get the char for non-terminal 
		switch (nt) {
		case 'S': row = 0; break; // if char is S
		case 'A': row = 1; break; // if char is A
		case 'B': row = 2; break; // if char is B
		}

		// read the char after nt one by one and put into temp
		while (line[charpos] != '\0') { 
			if (line[charpos] != '$' && line[charpos] != ' ') {
				// if the char is valid one, add to the vector
				temp.push_back(line[charpos]);
			}
			charpos++;
		}

		//determine the column of the rule
		x = temp[0]; // get the char after nt
		if (x == 'S') col = 0;
		else if (x == 'A') col = 1;
		else if (x == 'B') col = 2;
		else if (x == '0') col = 3;
		else if (x == '1') col = 4;
		M[row][col] = temp; // add temp vector to this position in Matrix 
		temp.clear(); // empty the temp
		charpos = 1; // reset the charpos for next linse
	}

	// ** display the table
	char rows[] = { 'S','A','B' };
	char cols[] = { 'S','A','B','0','1' };
	int index;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 5; j++) {
			cout << "row " << rows[i] << " col " << cols[j] << " :";
			for (index = 0; index < M[i][j].size(); index++) {
				cout << M[i][j].at(index) << " ";
			}
			cout << endl;
		}
	}
}

// Method to display a stack
void display(vector<char> V)
{   
	//if a stack is empty
	if (V.empty()) {
		cout << "[ Empty ]" << endl;
	}
	else {
		for (int i = V.size() - 1; i >= 0; i--) {
			cout << V.at(i) << endl;
		}
	}
}

// Method to add rules into global stack
void addtostack(vector<char> V)
{
	cout << "Adding rhs of a rule to the stack" << endl << endl;
	// ** push V contents to the stack - be careful of the order
	int index;
	// push from end to begin of V, to make sure begin of V is the top of the stack
	for (index = V.size() - 1; index >= 0; index--) {
		stack.push_back(V[index]);
	}
}

// Main function
int main()
{
	readrules();
	string ss;
	cout << "Enter a string made of 0's and/or 1's: ";
	cin >> ss;

	// ** push 'S' onto the stack to start
	stack.push_back('S');
	cout << "Stack";
	// ** display the stack vertically
	cout << endl;
	display(stack);

	int i = 0;
	int row, col; 
	while (ss[i] != '\0')  // for each char of ss
	{
		cout << "--------------------------" << endl;
		cout << "current char is: " << ss[i] << endl;

		//case 1: char is not 0 or 1
		if (ss[i] != '0'&& ss[i] != '1') {
			cout << "invalid value error" << endl;
			break;
		}

		//case 2: compare char with element in stack
		//       2.1. if the top of stack is S, A or B
		char elem = stack[stack.size() - 1];
		if (elem == 'S' || elem == 'A' || elem == 'B') {
			//find the position of the rule
			if (elem == 'A') { row = 1; }
			else if (elem == 'B') { row = 2; }
			else if (elem == 'S') { row = 0; }

			if (ss[i] == '0') { col = 3; }
			else if (ss[i] == '1') { col = 4; }

			// 2.1.1 there's no such a rule
			if (M[row][col].empty()) {
				cout << "no such rule error" << endl;
				break;
			}
			// 2.1.2 find such a rule
			else { 
				stack.pop_back(); //remove the current top 
				addtostack(M[row][col]); // add rule into stack
				cout << "Stack" << endl;
				display(stack);
			}

			//check the current top of the stack, should be 0 or 1
			elem = stack[stack.size() - 1];

			cout << "-----------------------" << endl;
			cout << "current char is " << ss[i] << endl;

			// 2.1.3 input char is matched elem
			if (elem == ss[i]) { 
				cout << "matched!" << endl << endl;
				stack.pop_back(); // remove the current top
				cout << "Stack" << endl;
				display(stack);
			}
			// 2.1.4 input char is not matched elem
			else {
				cout << "mismatch error" << endl;
				break;
			}
		}
		//  2.2. if the top of stack is 0 or 1
		else if (elem == '0' || elem == '1') {
			// 2.2.1 if matched
			if (ss[i] == elem) {
				cout << "matched!" << endl << endl;
				stack.pop_back();
				cout << "Stack" << endl;
				display(stack);
			}
			// 2.2.2 if not matched
			else {
				cout << "mismatch error" << endl;
				break;
			}
		}
		i++;

		// if the stack is empty but string is not 
		if (stack.empty() && ss[i] != '\0') {
			cout << "-----------------------" << endl;
			cout << "current char is " << ss[i] << endl;
			cout << "error - stack is empty " << endl;
			break;
		}
		// if the string is empty but stack is not
		if (!stack.empty() && ss[i] == '\0') {
			cout << "-----------------------" << endl;
			cout << "Failure - input too short" << endl;
		}
		// if both string and stack are empty
		else if (stack.empty() && ss[i] == '\0') {
			cout << "-----------------------" << endl;
			cout << "Success!!" << endl;
		}
	}
}
