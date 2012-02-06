#ifndef FILEMANAGER_H
#define FILEMANAGER_H

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
#include <sys/stat.h>
#include <fstream>
#include "File.h"
#include "Engine.h"

#ifdef WINDOW
    #include <io.h>
#endif


using namespace std;



class FileManager {

private:
	File m_file;

        // list of files with thier respective data read into buffer
        map<string,string > m_vListOfBuffers;
	map<string,string >::iterator m_mIter;
	string m_sBuffer;
	set<string> m_setDisplayFiles; // files to display and choose
        set<string> m_setTargetFiles; // files to apply algorithm to
	set<string>::iterator m_setIter;
	static string m_sRelativePathName;

        // load content of files from target list into buffer
        // called by loadAllFileIntoBuffer
        int mapFileNameToBuffer(const string& s_FullFilePath);

public:
        // clear target list
        void clearTargetFile();

        // clear content of files that had been read into buffer
        void clearFileBuffer();

        // add files to target list
        // call clearTargetFile() first before adding new files for new project
        void addTargetFile(const string& fileName);

        // remove files from target list
        void removeTargetFile(const string& fileName);

        // display list of directories to choose from to add to target list
        void showDisplayFile();

        // show target list
        void showTargetFile();

        // read directories recursively for files from target list
        // load content of files from target list into buffer
        // call clearFileBuffer() before using this function to load new files into buffer
        void loadAllFileIntoBuffer();

        // create a folder and output buffer from target list
        void writeDirectory(const string& directoryName,
                            map<string,string>& listOfBuffers,
                            const string& projectName,
                            const string& headerFileName = "",
                            const string& adminFileName = "",
			    bool b_addHeaderFile = false);

        // get buffers that contains content of files from target list
        void getListOfBuffers(map<string,string >& listOfBuffers);

        // get display list
        void getDisplayFile(set<string>& set_input);

        // get target list
        void getTargetFile(set<string>& set_input);


        void writeFile(const string& s_input, const string& fileName);
        void loadFile(const string& fileName, vector<string>& set_str);
        void loadFile(const string& fileName, set<string>& set_str);
        void loadFile(const string& fileName, string& s_str);
        void writeMappingToFile(map<string,string>& m_input, const string& fileName);
        void loadMappingFile(const string& fileName, string& s_output);
};


#endif
