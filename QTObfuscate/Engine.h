#ifndef ENGINE_H
#define ENGINE_H

#include "FileManager.h"
#include "mainwindow.h"
#include "PluginManager.h"


#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
#define WINDOW
#endif


#define ADMIN_FILE_NAME         "ADMIN"
#define DEFAULT_PLUGIN_PATH     "./plugin/"
#define PLUGIN_INFO_PATH        "plugin/plugin.txt"
#define NOT_TO_LIST             "Config/notToWords.txt"
#define KEYWORDS_LIST           "Config/keywords.txt"
#define CONFIG_FILE           "Config/config.txt"

#define NUM_RANDOM               8
#define CANNOT_LOAD_PLUGIN       1
#define ALGORITHM_FAILED       2
#define SAFE_DELETE( p )         { if( p ) { delete ( p );     ( p ) = NULL; } }


class PluginManager;
class FileManager;

struct Options
{
        string s_projectName;
        string s_saveToDirectory;
        string s_headerFileName;
        string s_mappingFileName;
        string s_pluginFileName;
        string s_adminFileName;
        string s_newPluginPathName;
};

extern Options *userOptions;

class Engine {

private:
	FileManager* m_fileManager;
        MainWindow* m_mainwindow;
        PluginManager* m_pluginManager;
	bool m_loaded;
public:

	Engine();
	virtual ~Engine();

	FileManager *getFileManager();
        PluginManager *getPluginManager();
        MainWindow *getmainwindow();
};

extern Engine *g_engine;

#endif
