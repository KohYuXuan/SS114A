#include "Engine.h"

Options *userOptions = NULL;
Engine *g_engine = NULL;


Engine::Engine()
{
	// Indicate that the engine is not yet loaded.
	m_loaded = false;

	g_engine = this;

        userOptions = new Options;

	m_fileManager = new FileManager();

        //m_algorithm1 = new Algorithm1();

        //m_plugin = new Plugin1();
        //m_plugin2 = new Plugin2();
        m_pluginManager = new PluginManager();

        m_mainwindow = new MainWindow();



	m_loaded = true;
}

//-----------------------------------------------------------------------------
// The engine class destructor.
//-----------------------------------------------------------------------------
Engine::~Engine()
{
	// Ensure the engine is loaded.
	if( m_loaded == true )
	{
		// Destroy everything.
		SAFE_DELETE( m_fileManager );
                //SAFE_DELETE( m_algorithm1 );
                //SAFE_DELETE( m_plugin );
                //SAFE_DELETE( m_plugin2 );
                SAFE_DELETE( m_pluginManager );
                SAFE_DELETE( m_mainwindow );
                SAFE_DELETE( userOptions );
                SAFE_DELETE( g_engine );
	}
}

FileManager* Engine::getFileManager()
{
	return m_fileManager;
}
/*
Algorithm1* Engine::getAlgorithm()
{
	return m_algorithm1;
}

Plugin1* Engine::getPlugin()
{
	return m_plugin;
}
*/
MainWindow* Engine::getmainwindow()
{
	return m_mainwindow;
}
/*
Plugin2* Engine::getPlugin2()
{
        return m_plugin2;
}
*/
PluginManager*  Engine::getPluginManager()
{
        return m_pluginManager;
}
