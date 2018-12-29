#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>  
#include <string>
#include "scanner.cpp"
using namespace std;

// INSTRUCTION:  Complete all ** parts.
// You may use any method to connect this file to scanner.cpp
// that you had written.  
// You can copy scanner.cpp here by cp ../ScannerFiles/scanner.cpp .
// -----------------------------------------------------

// Beginning of parser
//=================================================
// File parser.cpp written by Runyu Xu
//=================================================

// ----- Utility and Globals -----------------------------------
string saved_lexeme;
tokentype saved_token;
bool token_available = false;
string choice;
ofstream errorfile;

// ** Need syntaxerror1 and syntaxerror2 functions (each takes 2 args)
// ** Be sure to put the name of the programmer above each function

// paser_function type for all parser functions 
enum parser_function { STORY, S, AFTER_SUBJECT, AFTER_NOUN, AFTER_OBEJCT, VERB1, TENSE, NOUN, BE };
// ** string parserName[30] = { }; for the display names of parser functions
string parserName[30] = { "story", "s", "after subject", "after noun", "after obejct", "verb", "tense", "noun", "be" };

// Syntaxerror1 for match fails
void syntaxError1(tokentype expected) {
	cout << endl << "SYNTAX ERROR: expected " << tokenName[expected] << " but found " << saved_lexeme << endl;
}

// Syntaxerror2 for switch default
void syntaxError2(parser_function function) {
	cout << endl << "SYNTAX ERROR: unexpected " << saved_lexeme << " found in " << parserName[function] << endl;
	exit(1);
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
			errorfile << "SYNTAX ERROR: expected " << tokenName[expected] << " but found " << saved_lexeme << "\n";
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

// Grammar: <after_object> ::= <noun> DESTINATION <verb> <tense> PERIOD | <verb> <tense> PERIOD
void after_object() {
	cout << "Processing <afterObject>" << endl;
	switch (next_token()) {
	case WORD1: case PRONOUN:
		noun();
		match(DESTINATION);
		verb();
		tense();
		match(PERIOD);
		break;
	case WORD2:
		verb();
		tense();
		match(PERIOD);
		break;
	default:
		syntaxError2(AFTER_OBEJCT);
	}
}

// Grammar: <after noun> ::= <be> PERIOD | DESTINATION <verb> <tense> PERIOD 
//                           | OBJECT <after obejct>
void after_noun() {
	cout << "Processing <afterNoun>" << endl;
	switch (next_token()) {
	case IS: case WAS:
		be();
		match(PERIOD);
		break;
	case DESTINATION:
		match(DESTINATION);
		verb();
		tense();
		match(PERIOD);
		break;
	case OBJECT:
		match(OBJECT);
		after_object();
		break;
	default:
		syntaxError2(AFTER_NOUN);
	}
}

// Grammar: <after subject>::= <verb> <tense> PERIOD | <noun> <after noun>
void after_subject() {
	cout << "Processing <afterSubject>" << endl;
	switch (next_token())
	{
	case WORD2:
		verb();
		tense();
		match(PERIOD);
		break;
	case WORD1: case PRONOUN:
		noun();
		after_noun();
		break;
	default:
		syntaxError2(AFTER_SUBJECT);
	}
}

// Grammar: <s>::= [CONNECTOR] <noun> SUBJECT <after subject>
void s() {
	cout << "Processing <s>" << endl;
	switch (next_token()) {
	case CONNECTOR:
		match(CONNECTOR);
		noun();
		match(SUBJECT);
		after_subject();
		break;
	default:
		noun();
		match(SUBJECT);
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
{
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
}// end
 //** require no other input files!
 //** syntax error EC requires producing errors.text of messages

