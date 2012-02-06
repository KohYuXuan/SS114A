
#include "PluginReverseDefine.h"

bool PluginReverseDefine::setUpFilesBufer()
{
        // clear previous content of files that had been read into buffer
	m_engine->getFileManager()->clearFileBuffer();

        // read directories recursively for files from target list
        // load content of files from target list into buffer
        // call clearFileBuffer() before using this function to load new files into buffer
	m_engine->getFileManager()->loadAllFileIntoBuffer();

	m_engine->getFileManager()->getListOfBuffers(m_listOfBuffers);

		// check empty buffer
	if (m_listOfBuffers.empty())
		return false;

	b_emptyBuffer = true;
		//vector<string> v_listOfHeaderFiles;
	for(m_mIter=m_listOfBuffers.begin(); m_mIter != m_listOfBuffers.end(); ++m_mIter) {

		if (!(*m_mIter).second.empty())
			b_emptyBuffer = false;

		if(b_emptyBuffer)
			return false;
	}
	return true;

}

bool PluginReverseDefine::setUpMapping(string& s_MapFileName, string& s_output)
{
        bool b_foundHeaderFileName = false;
        int i_startPos=0, i_endPos=0;
        string s_string, s_fileName, s_headefileName, s_tempFileName;
        set<string> set_fileName;
        set<string>::iterator m_setIter;
        map<string,string> m_mappingReverse;

        m_engine->getmainwindow()->getfullPathNamesList(set_fileName);

        for ( m_setIter=set_fileName.begin() ; m_setIter != set_fileName.end(); m_setIter++ ) {
                s_fileName = *m_setIter;
                i_startPos = s_fileName.find_last_of("/");
                i_endPos = s_fileName.size();
                s_tempFileName.assign(s_fileName, i_startPos+1, i_endPos-i_startPos-1);

                for(m_mIter=m_listOfBuffers.begin(); m_mIter != m_listOfBuffers.end(); ++m_mIter) {

                    if ( (*m_mIter).first.find(s_tempFileName) != string::npos
                         && (*m_mIter).first.find(s_MapFileName) != string::npos) {
                        if (s_tempFileName.compare(s_MapFileName) == 0)
                            s_headefileName =  s_fileName;
                        else
                            s_headefileName =  s_fileName + "/" + s_MapFileName;

                        b_foundHeaderFileName = true;
                        break;
                    }
                    if (b_foundHeaderFileName)
                        break;
                }
        }

            m_engine->getFileManager()->loadFile(s_headefileName, s_string);
            if (s_string.empty())
                return false;
            else {
                s_output = s_string;
                return true;
            }
}

bool PluginReverseDefine::algorithm() {

	string s_mapping, s_filename;
	map<string,string> m_output; // contains targeted filenames and final result to ouput to files
	map<string,string> m_outputNoHeader; // some .cpp contains .h files and these .h files are also being obfuscated
                                        // so those .cpp files will retain the .h files and will not have #include "ProjectName" included

	// save user choosen path into target list
    if(!setUpFile())
        return false;

    // load all files into buffer
    if(!setUpFilesBufer())
        return false;

    s_filename = "ProjectName.h";
    if(!setUpMapping(s_filename,s_mapping))
        return false;

    m_mappingReverseDefine.clear();

        // get mapping for list of defines in header files
    m_algorithm.getMappingReverse(s_mapping, m_mappingReverseDefine);

    if (m_mappingReverseDefine.empty())
        return false;

    // clear setting if this is another new project
    m_algorithm.clearAlgorithmBuffer();

	string s_input,s_sourceOutput, s_headerOutput;// s_mappingOutput;
	bool b_addHeaderFile;
    // list of headerName
    vector<string> v_headerName;

	// loop through the target list of files
	for(m_mIter=m_listOfBuffers.begin(); m_mIter != m_listOfBuffers.end(); ++m_mIter) {
        if ( (*m_mIter).first.find("ProjectName.h") != string::npos) {
            s_input.clear();
            s_input = (*m_mIter).second;
            m_algorithm.listOfInclude(s_input, v_headerName, m_mappingReverseDefine);
            continue;
        }

        if ((*m_mIter).first.find("ProjectNameMAP") != string::npos)
                continue;

	    b_addHeaderFile = false;
		s_sourceOutput.clear();
        s_input.clear();
		s_input = (*m_mIter).second;

		m_algorithm.algorithmMap(s_input, s_sourceOutput, m_mappingReverseDefine);
        s_input = s_sourceOutput;
		m_algorithm.format(s_input, s_sourceOutput);

        if (b_addHeaderFile)
            // insert targeted filenames and final result
            m_output.insert ( pair<string,string>((*m_mIter).first, s_sourceOutput ));
        else
            m_outputNoHeader.insert ( pair<string,string>((*m_mIter).first, s_sourceOutput));
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
        m_engine->getFileManager()->writeDirectory(directoryName, m_output, projectName,
                                                   headerFileName, "",true);

	if (!m_outputNoHeader.empty())
        m_engine->getFileManager()->writeDirectory(directoryName, m_outputNoHeader, projectName,
                                                   headerFileName, "",false);


    s_headerOutput.insert(0, "#ifndef XENUS_ENGINE\n#define XENUS_ENGINE\n");

    for (int i=0; i<v_headerName.size(); ++i)
        s_headerOutput += "\n#include " + v_headerName[i];

    s_headerOutput += "\n#endif\n";

    string s_headerFilePath;
    int i_pos;

    string s_fullHeaderPath;
    s_fullHeaderPath = directoryName;
    s_fullHeaderPath += "/" + headerFileName;
    m_engine->getFileManager()->writeFile(s_headerOutput, s_fullHeaderPath);
    m_output.insert(m_outputNoHeader.begin(),m_outputNoHeader.end());
    compileFiles(m_output,false);

    return true;
}
