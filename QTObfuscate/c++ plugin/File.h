#ifndef FILE_H
#define FILE_H

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


class File {

public:

    void loadFile(const string& fileName, string& v);

    void loadFile(const string& fileName, vector<string>& set_str);

    void loadFile(const string& fileName, set<string>& set_str);

    void writeFile(const string& s_input, const string& fileName);

    void writeMappingToFile(map<string,string>& m_input, const string& fileName);

    void loadMappingFile(const string& fileName, string& s_output);
};


#endif
