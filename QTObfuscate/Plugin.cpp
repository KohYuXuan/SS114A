#include "Plugin.h"

bool Plugin::setUpFile()
{
    string s_fileName;
	set<string> set_fileName;
	set<string>::iterator m_setIter;

        m_engine->getmainwindow()->getfullPathNamesList(set_fileName);
	m_engine->getFileManager()->clearTargetFile();

    if(set_fileName.empty())
        return false;

	for ( m_setIter=set_fileName.begin() ; m_setIter != set_fileName.end(); m_setIter++ ) {
                s_fileName = *m_setIter;
                m_engine->getFileManager()->addTargetFile(s_fileName);
	}

	return true;
}

void Plugin::setUpOutputFiles()
{
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
}

void Plugin::writeDirectory(map<string,string>& p_listOfBuffers, bool b_headerFile)
{
    // output to files
    m_engine->getFileManager()->writeDirectory(directoryName, p_listOfBuffers, projectName, headerFileName, adminPathName, b_headerFile);
}

void Plugin::compileFiles(map<string,string>& p_listOfBuffers, bool b_includeHeaderPath)
{
    string s_compileFileNames;
    map<string,string>::iterator m_mapIter;
    for ( m_mapIter=p_listOfBuffers.begin() ; m_mapIter != p_listOfBuffers.end(); m_mapIter++ ) {
            s_compileFileNames += " " + directoryName + (*m_mapIter).first;
    }

    if (b_includeHeaderPath)
        s_compileFileNames += " -I\"" + adminPathName + "\"";
    string s_outputPath;
    s_outputPath = "g++ -o " + directoryName +  "/" + projectName + ".exe";
    s_compileFileNames.insert(0,s_outputPath);
    int i_result = system(s_compileFileNames.c_str());
    cout <<  s_compileFileNames << endl;
    cout <<  i_result << endl;
}
