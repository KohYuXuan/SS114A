#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H


#include <string>
#include <iostream>
#include <dlfcn.h>
#include "Engine.h"
#include "Plugin.h"
using namespace std;


class PluginManager {

public:
    int loadPlugin(const std::string& pluginName);
};


#endif
