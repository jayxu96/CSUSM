#include<iostream>
#include<fstream>
#include<vector>
#include<string>
using namespace std;

//--------------------------------------
// CS421 File td.cpp for HW2B
// Your name: Runyu Xu
//--------------------------------------

// ** Complete this to fit the HW2B specification - look for **
// ** Must have the same types of tracing couts as my demo program.


// info on each DFA
struct info
{
	string name;  // token name
	int startstate;
	int finalstate;
};

info DFA[4];      // store up to 4 dfas' start and final
int  TRS[10][4];  // store trs's - states 0-9 and chars a b c d 

// ----- utility functions -----------------------

// displayTables function to display the table and DFAs
void displayTables()
{
	// ** display TRS nicely labeled
	cout << "         a b c d" << endl;
	for (int i = 0; i < 10; i++) {
		cout << "State " << i << ": ";
		for (int j = 0; j < 4; j++) {
			if (TRS[i][j] == -1) {
				cout << "  ";
			}
			else {
				cout << TRS[i][j] << " ";
			}
		}
		cout << endl;
	}
	// ** display DFA nicely labeled
	for (int i = 0; i < 4; i++) {
		if (!DFA[i].name.empty())
			cout << "Token " << DFA[i].name << ": " << DFA[i].startstate
			<< " is start and end is " << DFA[i].finalstate << endl;
	}
}// end of displayTables

// readTables function to read the table and DFAs from two txt files
void readTables()
{
	ifstream fin("trs.txt", ios::in);
	ifstream fin2("dfa.txt", ios::in);
	// Read the TRS
	int i, j, k;
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 4; j++) {
			fin >> TRS[i][j];
		}
	}
	// Read the DFAs
	for (int k = 0; k < 4; k++) {
		fin2 >> DFA[k].name >> DFA[k].startstate >> DFA[k].finalstate;
	}

	fin.close(); // close file trs.txt
	fin2.close(); // close file dfa.txt
}// end of readTables

// accept function to check if a DFA acceptes the word
bool accept(info dfa, string word)
{
	// ** does the dfa accept the word?
	int charpos = 0; // the postion of char in word
	int state = dfa.startstate; // the state of a char, initial with the start state
	cout << "Trying dfa " << dfa.name << "-------------" << endl;
	while (word[charpos] != '\0') // while not finish read the word
	{
		cout << "state: " << state << " char: " << word[charpos] << endl;
		// check if a char is the char for loop on the start state
		if (state == dfa.startstate && word[charpos] == dfa.name[0])
		{
			state = dfa.startstate;
		}
		//else check if it is the char to go to next state
		else
			if (state == dfa.startstate && word[charpos] == dfa.name[1])
			{
				state = dfa.finalstate;
			}
		//else the char is not in this DFA, change the state to -1
			else
			{
				state = -1;
			}
		cout << "new state: " << state << endl;
		if (state == -1) { // if the char is not in the DFA, return false
			return(false);
		}
		charpos++;
	}//end of while loop
	if (state == dfa.finalstate) return(true); //end in a final state
	else return(false);
} // end of accept


int main()
{
	cout << "This is a table driven scanner. Needs trs.txt and dfa.txt." << endl;
	cout << "States are 0 to 9 and chars are a to d" << endl;

	readTables();
	displayTables();
	cout << ".....done reading tables...." << endl;

	string word;
	while (true)
	{
		cout << "@@Enter a string: ";
		cin >> word;
		/*if (accept(DFA[0], word)) {
			cout << "Found token " << DFA[0].name << endl;
		}
		else
			if (accept(DFA[1], word)) {
				cout << "Found token " << DFA[1].name << endl;
			}
			else {
				cout << "Lexical error!" << endl;
			}*/
		// check the DFAs one by one until find the token or Lexical error
		for (int i = 0; i < 4; i++) {
			if (DFA[i].name.empty()) {
				cout << "Lexical error!" << endl;
				break;
			}
			if (accept(DFA[i], word)) {
				cout << "Found token " << DFA[i].name << endl;
				break;
			}
		}
		cout << "do control-C to quit" << endl;
	}
}

