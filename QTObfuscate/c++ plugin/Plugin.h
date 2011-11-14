#ifndef PLUGIN_H
#define PLUGIN_H

#include "Engine.h"

class Engine;
class Options;

class Plugin {
private:


protected:
    Engine* m_engine;
    Options *m_userOptions;


public:
    //Plugin(){m_engine = g_engine;}
    Plugin(){}
    virtual ~Plugin(){}

virtual void algorithm()=0;

    void setEngine(Engine* p_engine) {m_engine = p_engine;}
    void setUserOptions(Options *p_userOptions) {m_userOptions = p_userOptions;}


};

// the types of the class factories
typedef Plugin* create_t();
typedef void destroy_t(Plugin*);

#endif // PLUGIN_H
