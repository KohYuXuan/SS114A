#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <iostream>
#include <cstdio>
#include <string.h>
#include <sys/stat.h> //mkdir
#include <fstream>

#include "File.h"

using namespace std;
//template<typename t_ListOfFile, typename t_OneFile>
class FileManager {

private:
	File m_file;

	map<string,string > m_vListOfBuffers; // list of files with thier respective data read into buffer
	map<string,string >::iterator m_mIter;
	string m_sBuffer;
	set<string> m_setDisplayFiles; // files to display and choose
	set<string> m_setTargetFiles; // files to obfuscate
	set<string>::iterator m_setIter;

	static string m_sRelativePathName;

	//void readFile(const string& fileName, string& output, char delim='\n');
	//void writeFile(const string& fileName);
protected:

public:
       // FileManager(){}
       // ~FileManager(){}

//searchFile()// to check for file if it is already in memeory
        void clearTargetFile();
        void clearFileBuffer();
void addTargetFile(const string& fileName); // call clearTargetFile() first before adding new files for new project
void removeTargetFile(const string& fileName); // remove from buffer
void showDisplayFile();
void showTargetFile();
int readDirectory(const string& dir);
void loadAllFileIntoBuffer(); // call clearFileBuffer() before using this function to load new buffer
void writeDirectory(const string& directoryName, map<string,string>& listOfBuffers, const string& projectName, const string& headerFileName = "", const string& adminFileName = "");
void getListOfBuffers(map<string,string >& listOfBuffers);
void getDisplayFile(set<string>& set_input);
void getTargetFile(set<string>& set_input);
int mapFileNameToBuffer(const string& s_FullFilePath);
void writeFile(const string& s_input, const string& fileName);
void loadFile(const string& fileName, vector<string>& set_str);
void loadFile(const string& fileName, set<string>& set_str);
void loadFile(const string& fileName, string& s_str);
void writeMappingToFile(map<string,string>& m_input, const string& fileName);
void loadMappingFile(const string& fileName, string& s_output);

/*
	void readFile(const string& fileName);
	void readFile(const vector<const string >& v_listOfFileName, 
					string& output, char delim='\n');
	void readFile(const vector<const string >& v_listOfFileName, 
					vector<string >& output, char delim='\n');

	void writeFile(const string& fileName, const string& v_input);
	void writeFileWithFormat(const vector<string >& v_listOfFileName, const vector<string >& input);
	void writeFileWithSpaceRemovedconst vector<string >& v_listOfFileName, 
							const vector<string >& input);
	void writeFile(const vector<string >& v_listOfFileName, const vector<string >& input);
*/

};

