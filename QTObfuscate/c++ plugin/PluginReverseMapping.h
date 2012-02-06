
#include "Plugin.h"
#include "AlgorithmReverseMapping.h"


class PluginDefine : public Plugin {

private:
	AlgorithmDefine m_algorithm;

	bool b_emptyBuffer;
	vector<string> v_include; // list of #include
	map<string,string> m_mappingReverseDefine; // reverse defines in header files
	map<string,string> m_mappingReverseRandom; // reverse random string in source files
    map<string,string> m_listOfBuffers; // contains targeted filenames and the contents of the files to act on
    map<string,string>::iterator m_mIter;

public:

	PluginDefine() { }

    bool setUpFilesBufer();
    bool setUpMapping(string& s_filename, string& s_output);

	// carray out the algorithm
	virtual bool algorithm();


};


extern "C" Plugin* create() {
    return new PluginDefine;
}

extern "C" void destroy(Plugin* p) {
    delete p;
}

