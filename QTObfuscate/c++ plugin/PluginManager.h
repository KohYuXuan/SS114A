#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <string>
#include <iostream>
#include "Plugin.h"
#include <dlfcn.h>
#include "Engine.h"

using namespace std;

class PluginManager {
private:
        //FileManager m_fileManager;
        //File m_file;
        //Algorithm1 m_algorithm1;
        //mainwindow m_mainwindow;

        //ReverseAlgorithm m_algorithm;

       // map<string,string> m_mapping;
protected:

public:

    void loadPlugin(const std::string& pluginName);

    //virtual void algorithm()=0;


};


#endif
