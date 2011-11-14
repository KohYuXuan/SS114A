#include "ReverseAlgorithm.h"
#include "Plugin.h"

class Plugin2 : public Plugin {
private:
        //FileManager m_fileManager;
        //File m_file;
        //Algorithm1 m_algorithm1;
        //mainwindow m_mainwindow;

        ReverseAlgorithm m_algorithm;

        map<string,string> m_mapping;
protected:

public:

void setUpFile();

void setUpMapping();

virtual void algorithm();


};

extern "C" Plugin* create() {
    return new Plugin2;
}

extern "C" void destroy(Plugin* p) {
    delete p;
}
