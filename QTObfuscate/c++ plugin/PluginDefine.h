
#include "Plugin.h"
#include "AlgorithmDefine.h"

class PluginDefine : public Plugin {

private:
	AlgorithmDefine m_algorithm;

public:
	PluginDefine() { }

	// carry out the algorithm
	virtual bool algorithm();

};

extern "C" Plugin* create() {
    return new PluginDefine;
}

extern "C" void destroy(Plugin* p) {
    delete p;
}
