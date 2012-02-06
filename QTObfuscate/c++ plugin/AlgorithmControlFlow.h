#ifndef ALGORITHM_CONTROL_FLOW
#define ALGORITHM_CONTROL_FLOW

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <time.h>
#include <cstdlib>
#include <sstream>
#include <ctype.h>
#include <set>

using namespace std;

char rand_alnum();

#define NUM_OF_RANDOM_CHAR 6
#define RANDOM_NUM (rand() % 100000 + 1000)


class AlgorithmControlFlow
{

private:

    vector<pair<string,string> > m_levels;
    vector<pair<int,int> > m_breaks;
    vector<pair<int,int> > m_continues;
    string s_output;
    string m_saveToBuffer;
    string s_letters;

public:

	AlgorithmControlFlow()
    {
        srand ( time(NULL) );
        s_letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789";
    }

    void getOutputBuffer(string& s_output);
    void writeToBuffer(const string& s_input);

    std::string rand_alnum_str (std::string::size_type sz);
    string intToString(int i_input);

    void control_flow_flattening(string& s_input, set<string>& set_dataTypes);
    void flatten_block(string& s_block);
    void transform_block(string &s_input, int &i_entry, int i_exit);
    void transform_if(string &s_input, int i_entry, int i_exit, int i_numElseIf, bool b_elseExist);
    void transform_while(string &s_input, int i_entry, int i_exit);
    void transform_switch(string &s_input, int i_entry, int i_exit);
    void transform_do(string &s_input, int i_entry, int i_exit);
    void transform_for(string &s_input, int i_entry, int i_exit);
    void transform_try(string &s_input, int i_entry, int i_exit);
    void transform_sequence(string &s_input, int i_entry, int i_exit);

    void findFunction(const string& s_input, int& i_startPosition, int &i_endPosition);
    bool moveVariable(string& s_input, string& s_type, string& s_output);
    void stringToReplace(string& s_input, string& s_output, string s_toReplace, string& s_replaceWith);
    void searchEndBracket(const string& s_input, int i_startPosition, int &i_endPosition);
    void searchEndBraces(const string& s_input, int i_startPosition, int &i_endPosition);
    void searchEndPosition(const string& s_input, int i_startPosition, int &i_endPosition);
    void searchEndOfElseIf(const string& s_input, int i_startPosition, int& i_endPosition, int& i_numElseIf, bool& b_elseExist);

};

#endif
