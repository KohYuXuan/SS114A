
#include "PluginROT.h"



bool Plugin1::algorithm() {

	string s_words;
	vector<string> v_listOfWords; // list of words to act on for each files
	vector<string> v_wordsToDefine; // to convert c++ keywords to random string has to use #define
	map<string,string> m_listOfBuffers; // contains targeted filenames and the contents of the files to act on
	map<string,string> m_output; // contains targeted filenames and final result to ouput to files
	map<string,string> m_listOfUserVariables; // contains user defined variables
	map<string,string> m_keyWords; // contains c++ keywords
	map<string,string>::iterator m_mIter;
	string output; // contains the first result
	string outputSecondPass; // contains the final result
	string s_headerFile; // header file that contains list of #defined c++ keywords
	set<string> set_keywords; // list of c++ key words
	set<string> set_wordsNotTo; // list of words not to be obfuscated

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

	// load a list of c++ keywords
	m_engine->getFileManager()->loadFile(KEYWORDS_LIST, set_keywords);
	// load a list of words not to be obfuscated
	m_engine->getFileManager()->loadFile(NOT_TO_LIST, set_wordsNotTo);

	// loop through the target list of files
	for(m_mIter=m_listOfBuffers.begin(); m_mIter != m_listOfBuffers.end(); ++m_mIter) {

		s_words.clear();
		v_listOfWords.clear();

		s_words = (*m_mIter).second;

		// scan from buffer the contents of file for random words that needs to convert back to original names and add to vector<string>
		m_algorithm.addWordsToList(s_words,v_listOfWords);

		// seperate user defined variables names and c++ keywords
		m_algorithm.findWordsFromList(set_keywords,v_listOfWords, v_wordsToDefine);

		// remove from vector<string> list of words not found in MAP files which contains list of random strings and thier original names
		m_algorithm.removeWordsFromList(set_keywords ,v_listOfWords);
		m_algorithm.removeWordsFromList(set_wordsNotTo ,v_listOfWords);


		// map original names to random strings
		m_algorithm.mapping(m_listOfUserVariables,v_listOfWords);
		//m_algorithm.mapping(m_keyWords, v_wordsToDefine);

		// change original names to random strings
		m_algorithm.modiftString(output, s_words, v_listOfWords, m_listOfUserVariables);
		//m_algorithm.modiftString(outputSecondPass, output, v_wordsToDefine, m_keyWords);

		// convert string in between quotes to hex
		//m_algorithm.convertQuotedStringToHex(outputSecondPass);




		string s_formate = output;
int i_pos=output.size()/2, i_numNewLine=0;		
while (output.find("\n",i_pos) != string::npos) {
	i_pos = output.find("\n",i_pos+1);
	++i_numNewLine;
}
cout << i_numNewLine << endl;
		// format file by removing space and newline
		if (i_numNewLine < 3)
			m_algorithm.reverseFormatFile(s_formate,output);
		else
			m_algorithm.formatWords(output);

		// insert targeted filenames and final result
		m_output.insert ( pair<string,string>((*m_mIter).first, output ));
	}


	// check for empty output
	b_emptyBuffer = true;
	for(m_mIter=m_output.begin(); m_mIter != m_output.end(); ++m_mIter) {
		if (!(*m_mIter).second.empty())
			b_emptyBuffer = false;

		if(b_emptyBuffer)
			return false;
	}

/*
	string directoryName;
	string headerPathName;
	string headerFileName;
	string mappingFileName;
	string folderName;
	string projectName;
	string adminName;
	string adminPathName;
	string s_mappingFileName;

	adminName = ADMIN_FILE_NAME;
	directoryName = m_userOptions->s_saveToDirectory;
	projectName = m_userOptions->s_projectName;
	folderName = "/" + projectName;
	directoryName.append(folderName);
	headerFileName = projectName + ".h";
	adminPathName = m_userOptions->s_adminFileName;
	adminPathName += "/" + projectName + ADMIN_FILE_NAME + "/";
	headerPathName = adminPathName + headerFileName;
	s_mappingFileName = "/" + projectName + "MAP";
	mappingFileName =  adminPathName + s_mappingFileName;
*/
	// output to files
	//m_engine->getFileManager()->writeDirectory(directoryName, m_output, projectName, headerFileName, adminPathName);
	setUpOutputFiles();
	writeDirectory(m_output);
	m_algorithm.createHeaderFile(m_keyWords, s_headerFile);
        m_engine->getFileManager()->writeFile(s_headerFile, headerPathName);

        map<string,string> m_combinedMapping;
        m_combinedMapping = m_keyWords;
        m_combinedMapping.insert(m_listOfUserVariables.begin(),m_listOfUserVariables.end());

        m_engine->getFileManager()->writeMappingToFile(m_combinedMapping, mappingFileName);

        return true;
}







