
#include "Plugin.h"
#include "AlgorithmReverseDefine.h"

class PluginReverseDefine : public Plugin {

private:
	AlgorithmReverseDefine m_algorithm;

	bool b_emptyBuffer;
	vector<string> v_include; // list of #include
	map<string,string> m_mappingReverseDefine; // reverse defines in header files
	map<string,string> m_mappingReverseRandom; // reverse random string in source files
    map<string,string> m_listOfBuffers; // contains targeted filenames and the contents of the files to act on
    map<string,string>::iterator m_mIter;

public:

	PluginReverseDefine() { }

    bool setUpFilesBufer();
    bool setUpMapping(string& s_filename, string& s_output);

	// carry out the algorithm
	virtual bool algorithm();


};

extern "C" Plugin* create() {
    return new PluginReverseDefine;
}

extern "C" void destroy(Plugin* p) {
    delete p;
}
