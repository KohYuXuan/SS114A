#ifndef FF
#define FF


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
#define HEADERFILE "#include \"obsfun.h\"\n"
using namespace std;



class File {
private:

protected:

public:

void loadFile(const string& fileName, string& v);

void loadFile(const string& fileName, set<string>& set_str);

void writeFile(const string& s_input, const string& fileName);

void writeMappingToFile(map<string,string>& m_input, const string& fileName);

void loadMappingFile(const string& fileName, string& s_output);
};


#endif



