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

class ReverseAlgorithm {
private:

protected:

public:

    void setUpMapping(const string& s_input, map<string,string>& m_output);

    void reverseModiftString(string& output, const string& str, const vector<string>& v,map<string,string>& mymap);

    void formatFile(const string& s_input, string& s_output);


    void addWordsToList(const string& str,vector<string>& v);
    void mapping(map<string,string>& mymap,vector<string>& v);
    void modiftString(string& output, const string& str, const vector<string>& v,map<string,string>& mymap);
    void removeWordsFromList(map<string,string> set_input,vector<string>& vec);
    void findWordsFromList(set<string>& m_input, vector<string>& v_input, vector<string>& v_output);

    void skipQuote(const string& s_input, int& i_skip, char c_quote = '\"');

    void convertQuotedStringToHex(string& s_searchString);


void hexToString(const string& s, string& output);
};


