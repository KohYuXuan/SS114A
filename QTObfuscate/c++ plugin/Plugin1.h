//#include "Engine.h"
#include "Plugin.h"
#include "Algorithm1.h"


//#define ADMIN_FOLDER_NAME "/ADMIN/"


class Plugin1 : public Plugin {
private:
	//FileManager m_fileManager;
	//File m_file;
	//Algorithm1 m_algorithm1;

	//mainwindow m_mainwindow;
protected:
	Algorithm1 m_algorithm;


public:

Plugin1() { }
//Plugin1() { }

void setUpFile();

virtual void algorithm();


};

extern "C" Plugin* create() {
    return new Plugin1;
}

extern "C" void destroy(Plugin* p) {
    delete p;
}

