#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>  
#include <string>
using namespace std;

// INSTRUCTION:  Complete all ** parts.
// You may use any method to connect this file to scanner.cpp
// that you had written.  
// You can copy scanner.cpp here by cp ../ScannerFiles/scanner.cpp .
// -----------------------------------------------------
//=====================================================
// File scanner.cpp written by: Runyu Xu
//=====================================================

// --------- DFAs ---------------------------------

// ** MYTOKEN DFA to be replaced by the WORD DFA
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
			else if (s[charpos] == 'a' || s[charpos] == 'i' || s[charpos] == 'u' || s[charpos] == 'e' ||
				s[charpos] == 'o' || s[charpos] == 'I' || s[charpos] == 'E')
				state = 1;
			else if (s[charpos] == 'd' || s[charpos] == 'j' || s[charpos] == 'w' || s[charpos] == 'y' || 
				s[charpos] == 'z')
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
			else if (s[charpos] == 'a' || s[charpos] == 'i' || s[charpos] == 'u' || s[charpos] == 'e' || 
				s[charpos] == 'o' || s[charpos] == 'I' || s[charpos] == 'E')
				state = 1;
			else if (s[charpos] == 'd' || s[charpos] == 'j' || s[charpos] == 'w' || s[charpos] == 'y' ||
				s[charpos] == 'z')
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
			else if (s[charpos] == 'a' || s[charpos] == 'i' || s[charpos] == 'u' || s[charpos] == 'e' ||
				s[charpos] == 'o' || s[charpos] == 'I' || s[charpos] == 'E')
				state = 1;
			else//ERROR: Does not meet any conditions based on state.
				return false;
		}
		else if (state == 3) {//reached after d,j,w,y,z -- followed by vowel
			if (s[charpos] == 'a' || s[charpos] == 'i' || s[charpos] == 'u' || s[charpos] == 'e' ||
				s[charpos] == 'o' || s[charpos] == 'I' || s[charpos] == 'E')
				state = 1;
			else//ERROR: Does not meet any conditions based on state.
				return false;
		}
		else if (state == 4) {//reached after s -- followed by vowel or h
			if (s[charpos] == 'h')
				state = 3;
			else if (s[charpos] == 'a' || s[charpos] == 'i' || s[charpos] == 'u' || s[charpos] == 'e' ||
				s[charpos] == 'o' || s[charpos] == 'I' || s[charpos] == 'E')
				state = 1;
			else//ERROR: Does not meet any conditions based on state.
				return false;
		}
		else if (state == 5) {//reached after t -- followed by vowel or s
			if (s[charpos] == 's')
				state = 3;
			else if (s[charpos] == 'a' || s[charpos] == 'i' || s[charpos] == 'u' || s[charpos] == 'e' || 
				s[charpos] == 'o' || s[charpos] == 'I' || s[charpos] == 'E')
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
enum tokentype { WORD1, WORD2, PERIOD, VERB, VERBNEG, VERBPAST, VERBPASTNEG, IS, WAS, OBJECT, SUBJECT,
	DESTINATION, PRONOUN, CONNECTOR, EOFM, ERROR };

// ** string tokenName[30] = { }; for the display names of tokens
string tokenName[30] = { "WORD1", "WORD2", "PERIOD", "VERB", "VERBNEG", "VERBPAST", "VERBPASTNEG", "IS",
"WAS", "OBJECT", "SUBJECT", "DESTINATION", "PRONOUN", "CONNECTOR", "EOFM", "ERROR" };

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
	else if (periodDFA(w)) {
		a = PERIOD;
	}
	else if (w == "eofm") {
		a = EOFM;
	}
	else {
		// Generated a lexical error
		cout << endl << "Lexical error: " << w << " is not a valid token." << endl;
		a = ERROR;
	}

	return 0;
}//the end of scanner

// Beginning of parser
//=================================================
// File parser.cpp written by Runyu Xu
//=================================================

// ----- Utility and Globals -----------------------------------
string saved_lexeme;
string saved_EWord;
tokentype saved_token;
bool token_available = false;
string choice;
ofstream errorfile;
ofstream translatedfile;
string JapWord[47];
string EngWord[47];

// ** Need syntaxerror1 and syntaxerror2 functions (each takes 2 args)
// ** Be sure to put the name of the programmer above each function

// paser_function type for all parser functions 
enum parser_function { STORY, S, AFTER_SUBJECT, AFTER_NOUN, AFTER_OBEJCT, VERB1, TENSE, NOUN, BE };
// ** string parserName[30] = { }; for the display names of parser functions
string parserName[30] = { "story", "s", "after subject", "after noun", "after obejct", "verb", "tense",
"noun", "be" };

// Syntaxerror1 for match fails
void syntaxError1(tokentype expected) {
	cout << endl << "SYNTAX ERROR: expected " << tokenName[expected] << " but found " << saved_lexeme 
		<< endl;
}

// Syntaxerror2 for switch default
void syntaxError2(parser_function function) {
	cout << endl << "SYNTAX ERROR: unexpected " << saved_lexeme << " found in " << parserName[function] 
		<< endl;
	exit(1);
}

// getEword to get the E word
void getEWord() {
	for (int i = 0; i < 47; i++) {
		if (saved_lexeme == JapWord[i]) {
			saved_EWord = EngWord[i];
			return;
		}
	}
	saved_EWord = saved_lexeme;
	return;
}

// gen to generate format element for each input sentence
void gen(string token) {
	if (token == "TENSE") {
		translatedfile << "TENSE: " << tokenName[saved_token] << endl << endl;
	}
	else {
		translatedfile << token << ": " << saved_EWord << endl;
	}

}
// ** Need the updated match and next_token (with 2 global vars)
// ** Be sure to put the name of the programmer above each function
//    Looks ahead to see what token comes next from the scanner.
//    HOW: checks first to see if the token_available flag is false.
//                If so, saved_token gets scanner() result.
//                and the flag is set true.
//    A token is grabbed but is not eaten up.
//    Returns the saved_token
tokentype next_token() {
	if (!token_available) {
		scanner(saved_token, saved_lexeme); //call scanner to get new token
		token_available = true;
	}
	return saved_token;
}

//  Checks and eats up the expected token.
//  HOW: checks to see if expected is different from next_token()
//              and if so, generates a syntax error and handles the error
//              else token_available becomes false (eat up) and returns true.
bool match(tokentype expected) {
	if (next_token() != expected)
	{
		syntaxError1(expected); // call syntax error1 message
								//EC part: to skip token or assume correct token was there
		if (errorfile.is_open()) {
			errorfile << "SYNTAX ERROR: expected " << tokenName[expected] << " but found " << 
				saved_lexeme << "\n";
		}
		cout << "Skip or replace the token? (s or r) ";
		cin >> choice;
		if (choice == "s") { // skip a token, check if next token is matched expected
			token_available = false;
			match(expected);
		}
		else if (choice == "r") {// replace a token, assume it is matched expected
			token_available = false;
			cout << "Matched " << tokenName[expected] << endl;
		}
	}
	else
	{
		token_available = false;
		cout << "Matched " << tokenName[expected] << endl;
		return true;
	}
}

// ----- RDP functions - one per non-term -------------------

// ** Make each non-terminal into a function here
// ** Be sure to put the corresponding grammar rule above each function

// Grammar: <tense> ::= VERBPAST | VERBPASTNEG | VERB | VERBNEG
void tense() {
	cout << "Processing <tense>" << endl;
	switch (next_token()) {
	case VERBPAST:
		match(VERBPAST);
		break;
	case VERBPASTNEG:
		match(VERBPASTNEG);
		break;
	case VERB:
		match(VERB);
		break;
	case VERBNEG:
		match(VERBNEG);
		break;
	default:
		syntaxError2(TENSE);
	}
}

// Grammar: <be> ::= IS | WAS
void be() {
	cout << "Processing <be>" << endl;
	switch (next_token()) {
	case IS:
		match(IS);
		break;
	case WAS:
		match(WAS);
		break;
	default:
		syntaxError2(BE);
	}
}

// Grammar: <verb> ::= WORD2
void verb() {
	cout << "Processing <verb>" << endl;
	match(WORD2);
}

// Grammar: <noun> ::= WORD1 | PRONOUN
void noun() {
	cout << "Processing <noun>" << endl;
	switch (next_token()) {
	case WORD1:
		match(WORD1);
		break;
	case PRONOUN:
		match(PRONOUN);
		break;
	default:
		syntaxError2(NOUN);
	}
}

// Grammar: <after_object> ::= <noun> #getEword# DESTINATION #gen("TO")# <verb> #getEword# 
//                             #gen("ACTION")# <tense> #gen("TENSE")# PERIOD 
//                             | <verb> #getEword# #gen("ACTION")# <tense> #gen("TENSE")# PERIOD
void after_object() {
	cout << "Processing <afterObject>" << endl;
	switch (next_token()) {
	case WORD1: case PRONOUN:
		noun();
		getEWord();
		match(DESTINATION);
		gen("TO");
		verb();
		getEWord();
		gen("ACTION");
		tense();
		gen("TENSE");
		match(PERIOD);
		break;
	case WORD2:
		verb();
		getEWord();
		gen("ACTION");
		tense();
		gen("TENSE");
		match(PERIOD);
		break;
	default:
		syntaxError2(AFTER_OBEJCT);
	}
}

// Grammar: <after noun> ::= <be> #gen("DESCRIPTION")# #gen("TENSE")# PERIOD | DESTINATION #gen("TO") 
//                           <verb> #getEword# #gen("ACTION)# <tense> #gen("TENSE")# PERIOD 
//                           | OBJECT #gen("OBJECT")# <after obejct>
void after_noun() {
	cout << "Processing <afterNoun>" << endl;
	switch (next_token()) {
	case IS: case WAS:
		be();
		gen("DESCRIPTION");
		gen("TENSE");
		match(PERIOD);
		break;
	case DESTINATION:
		match(DESTINATION);
		gen("TO");
		verb();
		getEWord();
		gen("ACTION");
		tense();
		gen("TENSE");
		match(PERIOD);
		break;
	case OBJECT:
		match(OBJECT);
		gen("OBJECT");
		after_object();
		break;
	default:
		syntaxError2(AFTER_NOUN);
	}
}

// Grammar: <after subject>::= <verb> #getEword# #gen("ACTION")# <tense> #gen("TENSE")# PERIOD |
//                             <noun> #getEWord# <after noun>
void after_subject() {
	cout << "Processing <afterSubject>" << endl;
	switch (next_token())
	{
	case WORD2:
		verb();
		getEWord();
		gen("ACTION");
		tense();
		gen("TENSE");
		match(PERIOD);
		break;
	case WORD1: case PRONOUN:
		noun();
		getEWord();
		after_noun();
		break;
	default:
		syntaxError2(AFTER_SUBJECT);
	}
}

// Grammar: <s>::= [CONNECTOR #getEword# #gen("CONNECTOR")#] <noun> #getEword# SUBJECT
//                 #gen("ACTOR")# <after subject>
void s() {
	cout << "Processing <s>" << endl;
	switch (next_token()) {
	case CONNECTOR:
		match(CONNECTOR);
		getEWord();
		gen("CONNECTOR");
		noun();
		getEWord();
		match(SUBJECT);
		gen("ACTOR");
		after_subject();
		break;
	default:
		noun();
		getEWord();
		match(SUBJECT);
		gen("ACTOR");
		after_subject();
		break;
	}
}

// Grammar: <story> ::= <s> {<s>}
void story() {
	cout << "Processing <story>" << endl << endl;
	s();
	while (true) {
		if (next_token() == EOFM) {
			cout << endl << "Successfully parsed <story>." << endl;
			break;
		}
		s();

	}
}

//---------------------------------------

// The new test driver to start the parser
int main()
{   //load the lexicon dictionary and store to two arrays
	ifstream dictionary;
	dictionary.open("lexicon.txt");

	for (int i = 0; i < 47; i++) {
		dictionary >> JapWord[i];
		dictionary >> EngWord[i];
	}

	translatedfile.open("translated.txt");
	// Open file
	string filename;
	cout << "Enter the input file name: ";
	cin >> filename;
	fin.open(filename.c_str());
	// Syntax EC: write error messages to errors.txt file
	cout << "Do you want to trace error messages? (y or n)";
	cin >> choice;
	if (choice == "y") {
		errorfile.open("errors.txt", ios::app);
	}
	//** calls the <story> to start parsing
	//** closes the input file 
	story();
	errorfile.close();
	fin.close();
	dictionary.close();
	translatedfile.close();
}// end
 //** require no other input files!
 //** syntax error EC requires producing errors.text of messages


