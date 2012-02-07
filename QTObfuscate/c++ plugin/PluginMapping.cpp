
#include "PluginMapping.h"

bool PluginMapping::algorithm() {

	map<string,string> m_listOfBuffers; // contains targeted filenames and the contents of the files to act on
	map<string,string> m_output; // contains targeted filenames and final result to ouput to files
	map<string,string> m_outputNoHeader; // some .cpp contains .h files and these .h files are also being obfuscated
	set<string> set_keywords; // list of c++ key words
	map<string,string>::iterator m_mIter;

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

		// load a list of c++ keywords
	m_engine->getFileManager()->loadFile(KEYWORDS_LIST, set_keywords);

        // clear setting if this is another new project
    m_algorithm.clearAlgorithmBuffer();

        // set list of C++ keywords
	m_algorithm.setListOfKeywords(set_keywords);

        // check empty buffer
	if (m_listOfBuffers.empty())
		return false;

	bool b_emptyBuffer = true;
	vector<string> v_listOfHeaderFiles;
	for(m_mIter=m_listOfBuffers.begin(); m_mIter != m_listOfBuffers.end(); ++m_mIter) {

		if (!(*m_mIter).second.empty())
			b_emptyBuffer = false;

        if ((*m_mIter).first.find(".h") != string::npos)
			v_listOfHeaderFiles.push_back((*m_mIter).first);

		if(b_emptyBuffer)
			return false;
	}

	string s_input,s_sourceOutput, s_headerOutput, s_mappingOutput;
    map<string,string> m_mappingOutput; // list of mapping of random names and user defined variables
	bool b_addHeaderFile;

        // loop through the target list of files
	for(m_mIter=m_listOfBuffers.begin(); m_mIter != m_listOfBuffers.end(); ++m_mIter) {
	    b_addHeaderFile = true;
		s_sourceOutput.clear();
        s_input.clear();
		s_input = (*m_mIter).second;

		m_algorithm.algorithmMap(s_input, s_sourceOutput, s_headerOutput, v_listOfHeaderFiles, b_addHeaderFile);
		m_algorithm.getMappingList(m_mappingOutput);
		s_input = s_sourceOutput;
		m_algorithm.algorithmDefine(s_input, s_sourceOutput, s_headerOutput, v_listOfHeaderFiles, b_addHeaderFile);

        if (b_addHeaderFile)
            // insert targeted filenames and final result
            m_output.insert ( pair<string,string>((*m_mIter).first, s_sourceOutput ));
        else
            m_outputNoHeader.insert ( pair<string,string>((*m_mIter).first, s_sourceOutput ));
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

    if (!m_output.empty())
        writeDirectory(m_output, true);

	if (!m_outputNoHeader.empty())
        writeDirectory(m_outputNoHeader, false);

    s_headerOutput.insert(0, "#ifndef XENUS_ENGINE\n#define XENUS_ENGINE\n");
    s_headerOutput += "\n#endif\n";

    m_engine->getFileManager()->writeFile(s_headerOutput, headerPathName);

    for(m_mIter=m_mappingOutput.begin(); m_mIter != m_mappingOutput.end(); ++m_mIter) {
        s_mappingOutput += (*m_mIter).second + ";" + (*m_mIter).first + ";";
    }

    m_engine->getFileManager()->writeFile(s_mappingOutput, mappingFileName);

    m_output.insert(m_outputNoHeader.begin(), m_outputNoHeader.end());
    compileFiles(m_output);

    return true;
}
