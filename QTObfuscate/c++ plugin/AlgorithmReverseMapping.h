
// constraints
// modify string function works on 62 characters max
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
//#define RAND_NAME "NMXWVAYHBFED"
#define RAND_NAME "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define NUM_NAME 4
#define MAX_CHARACTER_ALLOWED 80
#define BLOCKS_PER_ROW 7


using namespace std;



class AlgorithmDefine {
private:

vector<string> m_listOfDefine;
vector<string> m_listOfInclude;
vector<string> m_listOfBlocks;
set<string> m_duplicateCheck;
set<string> m_setDuplicateMappingCheck;
set<string> m_listOfKeywords; // store C++ keywords
//map<string,string> m_duplicateRandCheck;
map<string,string> m_mappingOutput;
map<string,string>::iterator m_iter;


public:
	AlgorithmDefine(){srand ( time(NULL) );}

    // scan through buffers for random string to revert back to original names
	bool algorithmMap(string& s_input, string& s_sourceOutput, map<string,string>& m_mappingReverseDefine, bool b_rot = false);

    // map random string to original string
	void mapString(string& s_input, map<string,string>& m_mappingReverseDefine, bool b_rot = false);

    // format files by inserting tabs and spaces
	void format(string& s_input, string& s_output);

    // convert string stored in vector to a string
	void convertToString(vector<string>& v_input, string& s_output);

    void removeComment(string& s_input, int& i_skip);

    // remove double or single quoted string
	void skipQuote(const string& s_input, int& i_skip, char c_quote = '\"');

    // set list of C++ keywords
    void setListOfKeywords(set<string>& set_listOfKeywords);

    // clear buffer for another new project
    void clearAlgorithmBuffer();

    // get list of mapping
    void getMappingList(map<string, string>& s_output);

    // ROT13 : rotate by 13 algorithm
    string rot13(string s_input);

    // reverse list of #defines
    void getMappingReverse(string& s_input, map<string,string>& m_output);

    // scan for #include
    void listOfInclude(string& s_input, vector<string>& v_output);

    // get list of mapping of random names to original names
    void getMappingReverseRandom(string& s_input, map<string,string>& m_output);

    void formatWords(string& str_output);
    void removeSpace( string& str_output );
    void removeNewline( string& s_input, vector<string>& v_output);

};




