#include "FileManager.h"


string FileManager::m_sRelativePathName = "";

void FileManager::clearTargetFile()
{
    m_setTargetFiles.clear();
}

void FileManager::clearFileBuffer()
{
    m_vListOfBuffers.clear();
}

void FileManager::addTargetFile(const string& fileName)
{
	m_setTargetFiles.insert(fileName);
}

void FileManager::removeTargetFile(const string& fileName)
{
	if(m_setTargetFiles.empty())
		return;

	m_setIter = m_setTargetFiles.find(fileName);
	if(m_setIter != m_setTargetFiles.end())
		m_setTargetFiles.erase (m_setIter);
}

void FileManager::showDisplayFile()
{
	for ( m_setIter=m_setDisplayFiles.begin() ; m_setIter != m_setDisplayFiles.end(); m_setIter++ )
		cout << *m_setIter<< endl;
}

void FileManager::showTargetFile()
{
	for ( m_setIter=m_setTargetFiles.begin() ; m_setIter != m_setTargetFiles.end(); m_setIter++ )
		cout << *m_setIter << endl;
}

void FileManager::loadAllFileIntoBuffer()
{
	string s_FileName;

	set<string>::iterator m_setIter;
	for ( m_setIter=m_setTargetFiles.begin() ; m_setIter != m_setTargetFiles.end(); m_setIter++ ) {
		s_FileName = *m_setIter;
		mapFileNameToBuffer(s_FileName);
	}
}

int FileManager::mapFileNameToBuffer(const string& s_FullFilePath)
{
        DIR *dp;
        struct dirent *dirp;
        string s_tempFullPathName;
        string s_oldDirectory;
        string s_buffer;
        size_t i_found;
        string s_parentDirectory;

        i_found = s_FullFilePath.find_last_of('/');
        if(i_found != string::npos)
                s_parentDirectory.assign(s_FullFilePath.begin()+i_found, s_FullFilePath.end());

        if((dp  = opendir(s_FullFilePath.c_str())) != NULL) {
        FileManager::m_sRelativePathName += s_parentDirectory;
                while ((dirp = readdir(dp)) != NULL) {
                        if( strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0 )
                                continue;

                        s_oldDirectory = FileManager::m_sRelativePathName;

                        s_tempFullPathName = s_FullFilePath;
                        s_tempFullPathName += "/" + string(dirp->d_name);

                        if(dirp->d_type == DT_DIR) { // if is directory
                                mapFileNameToBuffer(s_tempFullPathName);
                        }
                        else {
                                FileManager::m_sRelativePathName += "/" + string(dirp->d_name);
                                if(s_tempFullPathName.find(".cpp")!=string::npos ||
                                        s_tempFullPathName.find(".c")!=string::npos ||
                                        s_tempFullPathName.find(".h")!=string::npos ||
                                        s_tempFullPathName.find(".hpp")!=string::npos ||
                                        s_tempFullPathName.find("MAP")!=string::npos
                                  ) {
                                        m_file.loadFile(s_tempFullPathName, s_buffer);
                                        m_vListOfBuffers.insert ( pair<string,string>(FileManager::m_sRelativePathName,s_buffer) );
                                        s_buffer.clear();
                                    }
                        }
                        FileManager::m_sRelativePathName = s_oldDirectory;
                }
                FileManager::m_sRelativePathName.clear();
                closedir(dp);
                return 0;
        }
        else{
            if(s_FullFilePath.find(".cpp")!=string::npos ||
                    s_FullFilePath.find(".c")!=string::npos ||
                    s_FullFilePath.find(".h")!=string::npos ||
                    s_FullFilePath.find(".hpp")!=string::npos ||
                    s_FullFilePath.find("MAP")!=string::npos
              ) {
                    m_file.loadFile(s_FullFilePath, s_buffer);
                    m_vListOfBuffers.insert ( pair<string,string>(s_parentDirectory, s_buffer) );
                    s_buffer.clear();
                }
        }
        FileManager::m_sRelativePathName.clear();
        return 0;

}

void FileManager::writeDirectory(const string& directoryName, map<string,string>& listOfBuffers, const string& projectName, const string& headerFileName, const string& adminFileName, bool b_addHeaderFile)
{
	if(listOfBuffers.empty())
		return;

	string s_fileName;
	string s_buffer;
        string s_additionalDirectoryName;
        string s_FullDirectoryName;
        string s_headerFile;
        size_t i_firstPathSlash = 0, i_secondPathSlash = 0;


        #ifdef WINDOW
            _mkdir(directoryName.c_str());
            if(adminFileName!="")
                _mkdir(adminFileName.c_str());
        #else
            mkdir(directoryName.c_str(),0777);
            if(adminFileName!="")
                mkdir(adminFileName.c_str(),0777);
        #endif


	for ( m_mIter=listOfBuffers.begin() ; m_mIter != listOfBuffers.end(); m_mIter++ ) {
                if(headerFileName!="" && b_addHeaderFile) {
                    s_headerFile = "#include \"" + headerFileName + "\" \n";
                }


                i_firstPathSlash = (*m_mIter).first.find_first_of('/');
                i_secondPathSlash = (*m_mIter).first.find_first_of('/',i_firstPathSlash+1);
                while(i_secondPathSlash != string::npos) {
                    s_additionalDirectoryName = "";
                    s_additionalDirectoryName.assign((*m_mIter).first.begin(), (*m_mIter).first.begin()+i_secondPathSlash+1);
                    s_FullDirectoryName = directoryName;
                    s_FullDirectoryName.append(s_additionalDirectoryName);

                    #ifdef WINDOW
                        _mkdir(s_FullDirectoryName.c_str());
                    #else
                        mkdir(s_FullDirectoryName.c_str(),0777);
                    #endif

                    i_firstPathSlash = (*m_mIter).first.find_first_of('/',i_firstPathSlash+1);
                    i_secondPathSlash = (*m_mIter).first.find_first_of('/',i_secondPathSlash+1);
                }

                i_firstPathSlash =  0;
                i_secondPathSlash =  0;

                s_fileName = directoryName + (*m_mIter).first;
                if(headerFileName!="")
                    s_buffer = s_headerFile;

                s_buffer.append((*m_mIter).second);

                if(headerFileName=="") {
                    int i_pos=0;
                    i_pos  = s_buffer.find(projectName);

                    if(i_pos!=string::npos) {
                        int i_pos2 = s_buffer.rfind("#include", i_pos);
                        i_pos = s_buffer.find("\"", i_pos);
                        s_buffer.erase(s_buffer.begin()+i_pos2, s_buffer.begin()+i_pos+2);
                    }
                }
		m_file.writeFile(s_buffer, s_fileName);
		s_buffer.clear();
	}
}

void FileManager::getListOfBuffers(map<string,string>& listOfBuffers)
{
	if(m_vListOfBuffers.empty())
		return;

	listOfBuffers.insert(m_vListOfBuffers.begin(), m_vListOfBuffers.end());
}

void FileManager::getDisplayFile(set<string>& set_input)
{
	set_input.clear();
	set_input = m_setDisplayFiles;
}
void FileManager::getTargetFile(set<string>& set_input)
{
	set_input.clear();
	set_input = m_setTargetFiles;
}


void FileManager::writeFile(const string& s_input, const string& fileName)
{
	m_file.writeFile(s_input,fileName);
}

void FileManager::loadFile(const string& fileName, vector<string>& set_str)
{
        m_file.loadFile(fileName, set_str);
}

void FileManager::loadFile(const string& fileName, set<string>& set_str)
{
	m_file.loadFile(fileName, set_str);
}

void FileManager::loadFile(const string& fileName, string& s_str)
{
        m_file.loadFile(fileName, s_str);
}

void FileManager::writeMappingToFile(map<string,string>& m_input, const string& fileName)
{
        m_file.writeMappingToFile(m_input,fileName);
}

void FileManager::loadMappingFile(const string& fileName, string& s_output)
{
        m_file.loadMappingFile(fileName, s_output);
}
