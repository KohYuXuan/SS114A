#ifndef PLUGIN_H
#define PLUGIN_H

#include "Engine.h"


class Engine;
class Options;


class Plugin {

protected:
    Engine* m_engine;
    Options *m_userOptions;

    string directoryName;
    string headerPathName;
    string headerFileName;
    string mappingFileName;
    string folderName;
    string projectName;
    string adminName;
    string adminPathName;
    string s_mappingFileName;



public:
    Plugin(){}
    virtual ~Plugin(){}

    virtual bool algorithm()=0;

    void setEngine(Engine* p_engine) {m_engine = p_engine;}
    void setUserOptions(Options *p_userOptions) {m_userOptions = p_userOptions;}



	// save user choosen path into target list
    bool setUpFile();
    void setUpOutputFiles();
    void writeDirectory(map<string,string>& p_listOfBuffers, bool b_headerFile = false);
    void compileFiles(map<string,string>& p_listOfBuffers, bool b_includeHeaderPath = true);



};

typedef Plugin* create_t();
typedef void destroy_t(Plugin*);


#endif // PLUGIN_H
