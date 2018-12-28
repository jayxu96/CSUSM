#include<iostream>
#include<fstream>
#include<string>
using namespace std;

//------------------------------------------------
// CS421 File fa.cpp for HW2B
// Your name: Runyu Xu
//------------------------------------------------

// ** Change this to fit the HW2B specification - look for **
// ** Must have the same types of tracing couts as mytoken.
// ** Must complete the ** comments with RE

// ---------- DFAs follow -------------------------

// MYTOKEN DFA done by Rika Sensei has a sample
// This FA is for a b^+
bool mytoken(string s)
{
	int state = 0;
	int charpos = 0;

	cout << "Trying the mytoken machine..." << endl;

	while (s[charpos] != '\0')
	{
		cout << "current state: " << state << endl;
		cout << "character: " << s[charpos] << endl;

		if (state == 0 && s[charpos] == 'a')
			state = 1;
		else
			if (state == 1 && s[charpos] == 'b')
				state = 2;
			else
				if (state == 2 && s[charpos] == 'b')
					state = 2;
				else
				{
					cout << "I am stuck in state " << state << endl;
					return(false);
				}
		charpos++;
	}//end of while
  // where did I end up????
	if (state == 2) return(true);  // end in a final state
	else return(false);
}// end of mytoken


// IDENT DFA 
// This FA is for l (l | d | _)*
bool ident_token(string s)
{
	int state = 0;
	int charpos = 0;

	cout << "Trying the ident_token machine..." << endl;

	while (s[charpos] != '\0')
	{
		cout << "current state: " << state << endl;
		cout << "character: " << s[charpos] << endl;

		//               letter
		// state 0 ------------------> state 1
		if (state == 0 && (s[charpos] == 'a' || s[charpos] == 'b')) 
			state = 1;
		// loop on letter, digit or underscore
		//              state 1 
		else
			if (state == 1 && (s[charpos] == 'a' || s[charpos] == 'b')) 
				state = 1;
			else
				if (state == 1 && (s[charpos] == '2' || s[charpos] == '3'
					|| s[charpos] == '_')) 
					state = 1;
				else
				{
					cout << "I am stuck in state " << state << endl;
					return(false);
				}
		charpos++;
	}//end of while
	 // where did I end up????
	if (state == 1) return(true);  // end in a final state 
	else return(false);
}//end of ident


// REAL DFA 
// This FA is for d^*. d^+
bool real_token(string s)
{
	int state = 0;
	int charpos = 0;

	cout << "Trying the real_token machine..." << endl;

	while (s[charpos] != '\0')
	{
		cout << "current state: " << state << endl;
		cout << "character: " << s[charpos] << endl;
		//      loop on digit
		//        state 0
		if (state == 0 && (s[charpos] == '2' || s[charpos] == '3'))
			state = 0;
		//                  .
		// state 0 -----------------> state 1
		else
			if (state == 0 && s[charpos] == '.')
				state = 1;
		    //                       digit
		    //  state 1 ----------------------------> state 2
			else
				if (state == 1 && (s[charpos] == '2' || s[charpos] == '3'))
					state = 2;
		        //               loop on digit
		        //                 state 2            
				else
					if (state == 2 && (s[charpos] == '2' || s[charpos] == '3'))
						state = 2;
					else
					{
						cout << "I am stuck in state " << state << endl;
						return(false);
					}
		charpos++;
	}//end of while
	 // where did I end up????
	if (state == 2) return(true);  // end in a final state
	else return(false);
}//end of real


//INT DFA 
// This FA is for d^+
bool integer_token(string s)
{
	int state = 0;
	int charpos = 0;

	cout << "Trying the integer_token machine..." << endl;

	while (s[charpos] != '\0')
	{
		cout << "current state: " << state << endl;
		cout << "character: " << s[charpos] << endl;
		//                   digit
        // state 0 --------------------------------------> state 1
		if (state == 0 && (s[charpos] == '2' || s[charpos] == '3'))
			state = 1;
		//               loop on digit
		//                  state 1
		else
			if (state == 1 && (s[charpos] == '2' || s[charpos] == '3'))
				state = 1;
			else
			{
				cout << "I am stuck in state " << state << endl;
				return(false);
			}
		charpos++;
	}//end of while
	 // where did I end up????
	if (state == 1) return(true);  // end in a final state
	else return(false);
}// end of int


// -----------------------------------------------------

enum tokentype { ERROR, MYTOKEN, IDENT, REAL, INT };
int scanner(tokentype&, string&);  // to be called by main
fstream fin;   // file stream to use

// The test-driver -- NO NEED TO CHANGE THIS
int main()
{
	string fname;
	cout << "Enter the input file name:";
	cin >> fname;

	fin.open(fname.c_str(), fstream::in);

	string tokens[5] = { "ERROR", "MYTOKEN", "IDENT", "REAL", "INT" };
	tokentype thetype;
	string theword;

	while (true)  // keep on going 
	{
		scanner(thetype, theword);  // the paramers will be set by Scanner
		if (theword == "EOF") break;

		cout << "Type is:" << tokens[thetype] << endl << endl;
	}

	cout << "End of File encountered" << endl;
	fin.close();
}// end of main 


// Scanner sets the_type and w - TO BE COMPLETED 
int scanner(tokentype& the_type, string& w)
{

	// This goes through all machines one by one on the input string w

	cout << ".....Scanner was called..." << endl;

	fin >> w;  // grab next word from fain.txt
	cout << "Word is:" << w << endl;
	// if w is EOF, stop scanner the file
	if (w == "EOF") {
		cout << "Reach the end of the file" << endl;
		the_type = ERROR;
	}
	// else check in each token machine until find the type
	else {
		if (mytoken(w))
		{
			the_type = MYTOKEN;
		}
		else
			if (ident_token(w))
			{
				the_type = IDENT;
			}
			else
				if (real_token(w))
				{
					the_type = REAL;
				}
				else
					if (integer_token(w))
					{
						the_type = INT;
					}
					else //none of the FAs returned TRUE
					{
						cout << "Lexical Error: The string is not in my language" << endl;
						the_type = ERROR;
					}
	}

}//the very end of scanner




