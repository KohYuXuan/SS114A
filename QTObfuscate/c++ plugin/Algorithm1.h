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



#define NUM_RANDOM 8
//#define HEADERFILE "#include \"obsfun.h\"\n"
using namespace std;



char rand_alnum();

class Algorithm1 {
private:

protected:

public:
Algorithm1(){srand ( time(NULL) );}
/*
void loadFile(const string& fileName, string& s_output);

void loadFile(const string& fileName, set<string>& set_str);

void writeFile(const string& s_input, const string& fileName);
*/
void addWordsToList(const string& str,vector<string>& v);

//char rand_alnum();
std::string rand_alnum_str (std::string::size_type sz);

void mapping(map<string,string>& mymap,vector<string>& v);
void modiftString(string& output, const string& str, const vector<string>& v,map<string,string>& mymap);



void removeWordsFromList(set<string> set_input,vector<string>& vec);
void findWordsFromList(set<string>& set_input, vector<string>& v_input, vector<string>& v_output);
void createHeaderFile(map<string,string>& mymap,string& s_output);
void skipQuote(const string& s_input, int& i_skip, char c_quote = '\"');
string charToHex(char& c);
void stringToHex(const string& s,string& output);
void convertQuotedStringToHex(string& s_searchString);
void removeSpace( string& str_output );
void removeNewline( string& s_input, vector<string>& v_output);
void formatWords(string& str_output);

};


