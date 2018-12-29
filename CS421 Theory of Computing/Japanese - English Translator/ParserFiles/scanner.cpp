#include<iostream>
#include<fstream>
#include<string>
using namespace std;

//=====================================================
// File scanner.cpp written by: Group Number: ** 21
//=====================================================

// --------- DFAs ---------------------------------

// ** MYTOKEN DFA to be replaced by the WORD DFA
// ** Done by: Austin Banks
// ** RE: (vowel | vowel n | consonant vowel | consonant vowel n | consonant-pair vowel | consonant-pair vowel n) ^ +

bool wordDFA(string s)
{
	int state = 0;
	int charpos = 0;

	//state# | state
	//  0    | q0
	//  1    | q0'
	//  2    | pair
	//  3    | conson
	//  4    | s
	//  5    | t
	//  6    | c
	while (s[charpos] != '\0')
	{
		if (state == 0) {//starting point & final state 1 -- takes all conditions except n
			if (s[charpos] == 's')
				state = 4;
			else if (s[charpos] == 't')
				state = 5;
			else if (s[charpos] == 'c')
				state = 6;
			else if (s[charpos] == 'a' || s[charpos] == 'i' || s[charpos] == 'u' || s[charpos] == 'e' || s[charpos] == 'o' || s[charpos] == 'I' || s[charpos] == 'E')
				state = 1;
			else if (s[charpos] == 'd' || s[charpos] == 'j' || s[charpos] == 'w' || s[charpos] == 'y' || s[charpos] == 'z')
				state = 3;
			else if (s[charpos] == 'b' || s[charpos] == 'g' || s[charpos] == 'h' || s[charpos] == 'k' ||
				s[charpos] == 'm' || s[charpos] == 'n' || s[charpos] == 'p' || s[charpos] == 'r')
				state = 2;
			else//ERROR: Does not meet any conditions based on state.
				return false;
		}
		else if (state == 1) {//reached after vowel & final state 2 -- takes all conditions
			if (s[charpos] == 'n')
				state = 0;
			else if (s[charpos] == 's')
				state = 4;
			else if (s[charpos] == 't')
				state = 5;
			else if (s[charpos] == 'c')
				state = 6;
			else if (s[charpos] == 'a' || s[charpos] == 'i' || s[charpos] == 'u' || s[charpos] == 'e' || s[charpos] == 'o' || s[charpos] == 'I' || s[charpos] == 'E')
				state = 1;
			else if (s[charpos] == 'd' || s[charpos] == 'j' || s[charpos] == 'w' || s[charpos] == 'y' || s[charpos] == 'z')
				state = 3;
			else if (s[charpos] == 'b' || s[charpos] == 'g' || s[charpos] == 'h' || s[charpos] == 'k' ||
				s[charpos] == 'm' || s[charpos] == 'n' || s[charpos] == 'p' || s[charpos] == 'r')
				state = 2;
			else//ERROR: Does not meet any conditions based on state.
				return false;
		}
		else if (state == 2) {//reached after b,g,h,k,m,n,p,r -- followed by vowel or y
			if (s[charpos] == 'y')
				state = 3;
			else if (s[charpos] == 'a' || s[charpos] == 'i' || s[charpos] == 'u' || s[charpos] == 'e' || s[charpos] == 'o' || s[charpos] == 'I' || s[charpos] == 'E')
				state = 1;
			else//ERROR: Does not meet any conditions based on state.
				return false;
		}
		else if (state == 3) {//reached after d,j,w,y,z -- followed by vowel
			 if (s[charpos] == 'a' || s[charpos] == 'i' || s[charpos] == 'u' || s[charpos] == 'e' || s[charpos] == 'o' || s[charpos] == 'I' || s[charpos] == 'E')
			state = 1;
			else//ERROR: Does not meet any conditions based on state.
				return false;
		}
		else if (state == 4) {//reached after s -- followed by vowel or h
			if (s[charpos] == 'h')
				state = 3;
			else if (s[charpos] == 'a' || s[charpos] == 'i' || s[charpos] == 'u' || s[charpos] == 'e' || s[charpos] == 'o' || s[charpos] == 'I' || s[charpos] == 'E')
				state = 1;
			else//ERROR: Does not meet any conditions based on state.
				return false;
		}
		else if (state == 5) {//reached after t -- followed by vowel or s
			if (s[charpos] == 's')
				state = 3;
			else if (s[charpos] == 'a' || s[charpos] == 'i' || s[charpos] == 'u' || s[charpos] == 'e' || s[charpos] == 'o' || s[charpos] == 'I' || s[charpos] == 'E')
				state = 1;
			else//ERROR: Does not meet any conditions based on state.
				return false;
		}
		else if (state == 6) {//reached after c -- followed by h
			if (s[charpos] == 'h')
				state = 3;
			else//ERROR: Does not meet any conditions based on state.
				return false;
		}

		charpos++;
	}//end of while

	 // where did I end up????
	if (state == 0 || state == 1) return(true);  // end in a final state
	else return(false);
}

// ** Add the PERIOD DFA here
// ** Done by: Austin Banks
//returns true if string is a period, false otherwise
bool periodDFA(string s)
{
	if (s[0] == '.')
		return true;
	else
		return false;
}


// -----  Tables -------------------------------------

// ** Update the tokentype to be WORD1, WORD2, PERIOD, ERROR, etc.
// Feel free to add a tokentype for the end-of-file marker.
enum tokentype { WORD1, WORD2, PERIOD, VERB, VERBNEG, VERBPAST, VERBPASTNEG, IS, WAS, OBJECT, SUBJECT, DESTINATION, PRONOUN, CONNECTOR, EOFM, ERROR };

// ** string tokenName[30] = { }; for the display names of tokens
string tokenName[30] = {"WORD1", "WORD2", "PERIOD", "VERB", "VERBNEG", "VERBPAST", "VERBPASTNEG", "IS", "WAS", "OBJECT", "SUBJECT", "DESTINATION", "PRONOUN", "CONNECTOR", "EOFM", "ERROR"};

// ** Need the reservedwords table to be set up here.
// ** Do not require any file input for this.
// ** a.out should work without any additional files.
string reservedwords[20][2] = {
	"masu", "VERB",
	"masen", "VERBNEG",
	"mashita", "VERBPAST",
	"masendeshita", "VERBPASTNEG",
	"desu", "IS",
	"deshita", "WAS",
	"o", "OBJECT",
	"wa", "SUBJECT",
	"ni", "DESTINATION",
	"watashi", "PRONOUN",
	"anata", "PRONOUN",
	"kare", "PRONOUN",
	"kanojo", "PRONOUN",
	"sore", "PRONOUN",
	"mata", "CONNECTOR",
	"soshite", "CONNECTOR",
	"shikashi", "CONNECTOR",
	"dakara", "CONNECTOR",
	"eofm", "EOFM"
};

// ------------ Scaner and Driver -----------------------

ifstream fin;  // global stream for reading from the input file

			   // Scanner processes only one word each time it is called
			   // Gives back the token type and the word itself
			   // ** Done by: Tyler Stratford
int scanner(tokentype& a, string& w)
{

	// ** Grab the next word from the file via fin
	fin >> w;
	cout << "Scanner called using word: " << w << endl;
	/*
	2. Call the token functions one after another (if-then-else)
	And generate a lexical error message if both DFAs failed.
	Let the token_type be ERROR in that case.
	3. Make sure WORDs are checked against the reservedwords list
	If not reserved, token_type is WORD1 or WORD2.
	4. Return the token type & string  (pass by reference)
	*/

	if (wordDFA(w)) {
		bool isreserved = false;

		// Check the grabbed string against the reserved words table
		for (int i = 0; i < 20; i++) {
			if (w == reservedwords[i][0]) {	// w was found to be reserved, assign the correct token type

				if (reservedwords[i][1] == "VERB")
					a = VERB;
				else if (reservedwords[i][1] == "VERBNEG")
					a = VERBNEG;
				else if (reservedwords[i][1] == "VERBPAST")
					a = VERBPAST;
				else if (reservedwords[i][1] == "VERBPASTNEG")
					a = VERBPASTNEG;
				else if (reservedwords[i][1] == "IS")
					a = IS;
				else if (reservedwords[i][1] == "WAS")
					a = WAS;
				else if (reservedwords[i][1] == "OBJECT")
					a = OBJECT;
				else if (reservedwords[i][1] == "SUBJECT")
					a = SUBJECT;
				else if (reservedwords[i][1] == "DESTINATION")
					a = DESTINATION;
				else if (reservedwords[i][1] == "PRONOUN")
					a = PRONOUN;
				else if (reservedwords[i][1] == "CONNECTOR")
					a = CONNECTOR;
				else if (reservedwords[i][1] == "EOFM")
					a = EOFM;
				isreserved = true;
				break;
			}
		}
		// if the string was not found in reserved words table, the string must be either WORD1 or WORD2
		if (!isreserved) {
			if (w[w.length() - 1] == 'I' || w[w.length() - 1] == 'E')
				a = WORD2;
			else
				a = WORD1;
		}
	}
	else if(periodDFA(w)) {
		a = PERIOD;
	}
	else if (w == "eofm") {
		a = EOFM;
	}
	else {
			// Generated a lexical error
			cout << endl <<  "Lexical error: " << w << " is not a valid token." << endl;
			a = ERROR;
	}

	return 0;
}//the end of scanner

