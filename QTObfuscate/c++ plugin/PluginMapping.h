
#include "Plugin.h"
#include "AlgorithmMapping.h"

class PluginMapping : public Plugin {

private:
	AlgorithmMapping m_algorithm;
public:
	PluginMapping() { }
	// carry out the algorithm
	virtual bool algorithm();
};

extern "C" Plugin* create() {
    return new PluginMapping;
}

extern "C" void destroy(Plugin* p) {
    delete p;
}
