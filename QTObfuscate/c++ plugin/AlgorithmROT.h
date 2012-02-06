#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <stdio.h>
#include <locale>
#include <map>
#include <set>
#include <algorithm>
#include <cstdlib> 
#include <time.h>
#include <sstream>
using namespace std;


#define NUM_RANDOM 8

// generate random character
char rand_alnum();


class Algorithm1 {

public:
	Algorithm1(){srand ( time(NULL) );}

	void reverseFormatFile(const string& s_input, string& s_output);

	char rot13(char x);

	// generate random string
	std::string rand_alnum_str (std::string::size_type sz);

	// scan from buffer the contents of file for random words that needs to convert back to original names and add to vector<string>
	void addWordsToList(string& s_input,vector<string>& v_targetWords);

	// map original names to random strings
	void mapping(map<string,string>& m_input,vector<string>& v_targetWords);
	
	// change original names to random strings
	void modiftString(string& output, const string& s_input, const vector<string>& v_targetWords,map<string,string>& mymap);

	// remove from vector<string> list of words not found in MAP files which contains list of random strings and thier original names
	void removeWordsFromList(set<string> set_input,vector<string>& v_targetWords);

	// seperate user defined variables names and c++ keywords
	void findWordsFromList(set<string>& set_input, vector<string>& v_input, vector<string>& v_output);

	// header file that contains list of [#define <random_string> <c++_keywords> ]
	void createHeaderFile(map<string,string>& m_input, string& s_output);
	
	// convert string in between quotes to hex
	void convertQuotedStringToHex(string& s_searchString);

	// format file by removing space and newline
	void formatWords(string& str_output);

private:
	// string inbetween quote shall be hex
	void skipQuote(const string& s_input, int& i_skip, char c_quote = '\"');

	// called by convertQuotedStringToHex()
	string charToHex(char& c);
	void stringToHex(const string& s,string& output);

	// called by formatWords()
	void removeSpace( string& str_output );
	void removeNewline( string& s_input, vector<string>& v_output);

	// skip header file (i.e. #include <lib>)
	void skipHeader(const string& s_input, int& i_skip, char c_bracket); 

	// skip comment  
	void removeComment(string& s_input, int& i_skip);
};


