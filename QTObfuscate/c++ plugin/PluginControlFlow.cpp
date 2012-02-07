
#include "PluginControlFlow.h"

bool PluginControlFlow::algorithm() {

	map<string,string> m_listOfBuffers; // contains targeted filenames and the contents of the files to act on
	map<string,string> m_output; // contains targeted filenames and final result to ouput to files
	map<string,string>::iterator m_mIter;
	set<string> set_dataTypes; // list of data types

        // save user choosen path into target list
    if(!setUpFile())
        return false;

        // clear previous content of files that had been read into buffer
	m_engine->getFileManager()->clearFileBuffer();

        // read directories recursively for files from target list
        // load content of files from target list into buffer
        // call clearFileBuffer() before using this function to load new files into buffer
	m_engine->getFileManager()->loadAllFileIntoBuffer();

        // get buffers that contains content of files from target list
	m_engine->getFileManager()->getListOfBuffers(m_listOfBuffers);

        //load a list of data types
	m_engine->getFileManager()->loadFile(NOT_TO_LIST, set_dataTypes);

        // check empty buffer
	if (m_listOfBuffers.empty())
		return false;

	bool b_emptyBuffer = true;
	for(m_mIter=m_listOfBuffers.begin(); m_mIter != m_listOfBuffers.end(); ++m_mIter) {

		if (!(*m_mIter).second.empty())
			b_emptyBuffer = false;

		if(b_emptyBuffer)
			return false;
	}

	string s_input,s_sourceOutput;
        // loop through the target list of files
	for(m_mIter=m_listOfBuffers.begin(); m_mIter != m_listOfBuffers.end(); ++m_mIter) {
		s_sourceOutput.clear();
		s_input = (*m_mIter).second;
		m_algorithm.control_flow_flattening(s_input,set_dataTypes);
		m_algorithm.getOutputBuffer(s_sourceOutput);
		// insert targeted filenames and final result
		m_output.insert ( pair<string,string>((*m_mIter).first, s_sourceOutput ));
	}

        // check for empty output
	b_emptyBuffer = true;
	for(m_mIter=m_output.begin(); m_mIter != m_output.end(); ++m_mIter) {
		if (!(*m_mIter).second.empty())
			b_emptyBuffer = false;

		if(b_emptyBuffer)
			return false;
	}

	setUpOutputFiles();
	writeDirectory(m_output);
    compileFiles(m_output);

	return true;
}








