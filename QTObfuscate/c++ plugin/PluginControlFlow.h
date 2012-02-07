#ifndef PLUGIN_CONTROL_FLOW
#define PLUGIN_CONTROL_FLOW


#include "Plugin.h"
#include "AlgorithmControlFlow.h"


class PluginControlFlow : public Plugin {
private:
	AlgorithmControlFlow m_algorithm;
public:
	PluginControlFlow() { }
	// carry out the algorithm
	virtual bool algorithm();
};


extern "C" Plugin* create() {
    return new PluginControlFlow;
}

extern "C" void destroy(Plugin* p) {
    delete p;
}

#endif
