
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <fstream>

#include <algorithm>
#include <time.h>
#include <cstdlib>
#include <sstream>
#include <ctype.h>

#define ADD_REDUNDANCY 2
#define RAND_NAME "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define NUM_NAME 4
#define MAX_CHARACTER_ALLOWED 80
#define BLOCKS_PER_ROW 7

using namespace std;

char rand_alnum();

class AlgorithmMapping {

private:
    vector<string> m_listOfDefine;
    vector<string> m_listOfInclude;
    vector<string> m_listOfBlocks;
    set<string> m_duplicateCheck;
    set<string> m_setDuplicateMappingCheck;
    set<string> m_listOfKeywords; // store C++ keywords
    map<string,string> m_mappingOutput;
    map<string,string>::iterator m_iter;

public:
	AlgorithmMapping(){srand ( time(NULL) );}

    // scan buffers for randoms names to revert to original names using mapping file
	bool algorithmMap(string& s_input, string& s_sourceOutput, string& s_headerOutput,
                vector<string>& set_listOfHeaderFiles, bool& b_addHeaderFile);

   // scan buffers for randoms names to revert to original names using header file
	bool algorithmDefine(string& s_input, string& s_sourceOutput, string& s_headerOutput,
                vector<string>& set_listOfHeaderFiles, bool& b_addHeaderFile);

    // format files by removing spaces and tabs
	void format(vector<string>& v_input, string& s_output);

    // convert strings in a vector into a string
	void convertToString(vector<string>& v_input, string& s_output);

	string addSpace(int i_numSpaces);
	string rand_alnum_str (std::string::size_type sz);
	void skipQuote(const string& s_input, int& i_skip, char c_quote = '\"');
	void skipHeader(const string& s_input, int& i_skip, char c_bracket);

    // create transposition of names
	void modifyString(string& s_input, string& s_randOutput, string& s_output);

    // create substitution of names
	void mapString(string& s_input);

	// create random strings
	void randomiseString(string& s_inOutput, vector<int>& v_charPos);
	string nameGenerator(string& s_name, int i_num);

    // skip comment
	void removeComment(string& s_input, int& i_skip);

    // set list of C++ keywords
    void setListOfKeywords(set<string>& set_listOfKeywords);

    // clear buffer for another new project
    void clearAlgorithmBuffer();

    // get list of mapping
    void getMappingList(map<string, string>& s_output);

    // ROT13 : rotate by 13 algorithm
    string rot13(string s_input);

};
