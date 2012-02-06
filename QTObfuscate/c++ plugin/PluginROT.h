
#include "Plugin.h"
#include "AlgorithmROT.h"


class Plugin1 : public Plugin {

private:
	Algorithm1 m_algorithm;

public:

	Plugin1() { }



	// carray out the algorithm
	virtual bool algorithm();


};


extern "C" Plugin* create() {
    return new Plugin1;
}

extern "C" void destroy(Plugin* p) {
    delete p;
}

