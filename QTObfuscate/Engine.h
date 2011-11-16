#ifndef ENGINE_H
#define ENGINE_H


//#include "Algorithm1.h"
#include "FileManager.h"
//#include "Plugin1.h"
#include "mainwindow.h"
//#include "Plugin2.h"
#include "PluginManager.h"

#define SAFE_DELETE( p )       { if( p ) { delete ( p );     ( p ) = NULL; } }
//#define HEADERFILE "#include \"obsfun.h\"\n"
#define ADMIN_FILE_NAME "ADMIN"
#define NOT_TO_LIST "Config/notToWords.txt"
#define KEYWORDS_LIST "Config/keywords.txt"
class PluginManager;

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
        //Algorithm1* m_algorithm1;
        //Plugin1* m_plugin;
        //Plugin2* m_plugin2;
        MainWindow* m_mainwindow;
        PluginManager* m_pluginManager;

	bool m_loaded;
protected:

public:
	Engine();
	virtual ~Engine();

	FileManager *getFileManager();
        //Algorithm1 *getAlgorithm();
        //Plugin1 *getPlugin();
       // Plugin2 *getPlugin2();
        PluginManager *getPluginManager();
        MainWindow *getmainwindow();


};

extern Engine *g_engine;

#endif
