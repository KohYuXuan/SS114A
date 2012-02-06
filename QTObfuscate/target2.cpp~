#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include "abc.h"
using namespace std;

void loadFile(string& s_str)
{
    s_str.clear();
    ifstream in("target2.cpp");
	string words;
    int count=0;
    if(in.is_open()) {
        while (!in.eof()){
            getline(in,words);
            if(!words.empty())
            {
                s_str.append(words);
            }
        }
    }else
        {
		cout << 'a' << endl;
            cout<<"Error opening file!\n";
        }
	in.close();
}

int main()
{
	string s_str;
	loadFile(s_str);
	cout << "===================opening target2.cpp=======================" << endl;
	cout << s_str << endl;
}
