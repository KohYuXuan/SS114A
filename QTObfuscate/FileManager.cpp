#include "Engine.h"

#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
#define WINDOW
#endif

	//void readFile(const string& fileName, string& output, char delim='\n');
	//void writeFile(const string& fileName);



//searchFile()// to check for file if it is already in memeory
/*
FileManager::FileManager()
{
	//m_sRelativePathName="";
}
*/

//static string FileManager::m_sRelativePathName; 
string FileManager::m_sRelativePathName = "";


void FileManager::clearTargetFile()
{
    m_setTargetFiles.clear();
}

void FileManager::clearFileBuffer()
{
    m_vListOfBuffers.clear();
}


void FileManager::addTargetFile(const string& fileName) // add file to m_vListOfFile
{
	m_setTargetFiles.insert(fileName);
}

void FileManager::removeTargetFile(const string& fileName) // remove from buffer
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

int FileManager::readDirectory(const string& dir)
{
	DIR *dp;
	struct dirent *dirp;
	if((dp  = opendir(dir.c_str())) == NULL) {
		cout << "Error(" << errno << ") opening " << dir << endl;
		return errno;
	}

	while ((dirp = readdir(dp)) != NULL) {
		if( strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0 )
			m_setDisplayFiles.insert(string(dirp->d_name));
	}
	closedir(dp);
	return 0;
}
/*
void FileManager::readFile(const string& fileName)
{
	m_sBuffer.clear();
	ifstream in(fileName.c_str());
	string words;
	int count=0;
	if(in.is_open()) {
		while (!in.eof()){
			getline(in,words);
			if(!words.empty())
			{
				m_sBuffer.push_back(words);
			}
			}
	}else
	{
		cout<<"Error opening file!\n";
	}
	in.close();
for(int i=0;i<m_sBuffer.size();++i)
{
	cout << "FileManager::readFile" << m_sBuffer[i] << endl;
}

}
*/
/*
void FileManager::loadAllFileIntoBuffer()
{	
	if(m_setTargetFiles.empty())
		return;

	string s_FileName;
	for ( m_setIter=m_setTargetFiles.begin() ; m_setIter != m_setTargetFiles.end(); m_setIter++ ) {
		s_FileName = *m_setIter;
		m_file.loadFile(s_FileName, m_sBuffer);
		m_vListOfBuffers.insert ( pair<string,string>(s_FileName,m_sBuffer) );
	}
}
*/
void FileManager::loadAllFileIntoBuffer()
{
	string s_FileName;

	set<string>::iterator m_setIter;
	for ( m_setIter=m_setTargetFiles.begin() ; m_setIter != m_setTargetFiles.end(); m_setIter++ ) {
		s_FileName = *m_setIter;
		mapFileNameToBuffer(s_FileName); 
	}
}
/*
void FileManager::writeFile(const string& fileName, const string& v_input)
{
	//int j=v.size();
        ofstream out(fileName.c_str());
        for(int i=0;i<v_input.size();++i)
        {
            out << v_input[i] << " ";
            //if(i<j-1)       // no newline after last record output to file
            //{
                //out<<"\n";
            //}
            //out.flush();

        }
        out.close();
}
*/
void FileManager::writeDirectory(const string& directoryName, map<string,string>& listOfBuffers, const string& projectName, const string& headerFileName, const string& adminFileName)
{
    cout << "FileManager::writeDirectory : " << endl;
	if(listOfBuffers.empty())
		return;

	string s_fileName;
	string s_buffer;
        string s_additionalDirectoryName;
        string s_FullDirectoryName;
        string s_headerFile;
        size_t i_firstPathSlash = 0, i_secondPathSlash = 0;  //, i_findQuote = 0;



        #ifdef WINDOW
            mkdir(directoryName.c_str());
            if(adminFileName!="")
                mkdir(adminFileName.c_str());
        #else
            mkdir(directoryName.c_str(),0777);
            if(adminFileName!="")
                mkdir(adminFileName.c_str(),0777);
cout<<"directoryName========================="<<directoryName << endl;
cout<<"adminFileName=========================" <<adminFileName<< endl;
cout<<"make directory=========================" << endl;
        #endif
	for ( m_mIter=listOfBuffers.begin() ; m_mIter != listOfBuffers.end(); m_mIter++ ) {
   cout << "FileManager::before s_headerFile : " << endl;
                if(headerFileName!="") {
                    s_headerFile = "#include \"" + headerFileName + "\" \n";
                    // "#include \"obsfun.h\"\n"
                }
  cout << "FileManager::s_headerFile : " << s_headerFile << endl;
                i_firstPathSlash = (*m_mIter).first.find_first_of('/');
                i_secondPathSlash = (*m_mIter).first.find_first_of('/',i_firstPathSlash+1);
                while(i_secondPathSlash != string::npos) {
                    s_additionalDirectoryName = "";
                    s_additionalDirectoryName.assign((*m_mIter).first.begin(), (*m_mIter).first.begin()+i_secondPathSlash+1);
                    s_FullDirectoryName = directoryName;
                    s_FullDirectoryName.append(s_additionalDirectoryName);

                    //i_findQuote = s_headerFile.find('"');
                    //s_headerFile.insert(i_findQuote+1, "../");
                    #ifdef WINDOW
                        mkdir(s_FullDirectoryName.c_str());
                    #else
                        mkdir(s_FullDirectoryName.c_str(),0777);
                    #endif
                    i_firstPathSlash = (*m_mIter).first.find_first_of('/',i_firstPathSlash+1);
                    i_secondPathSlash = (*m_mIter).first.find_first_of('/',i_secondPathSlash+1);
                }
                i_firstPathSlash =  0;
                i_secondPathSlash =  0;
               // else {
                   // s_additionalDirectoryName = "";
                //}
 cout << "FileManager::writeDirectory2 : " << endl;




                s_fileName = directoryName + (*m_mIter).first;
                if(headerFileName!="")
                    s_buffer = s_headerFile;
                s_buffer.append((*m_mIter).second);


                if(headerFileName=="") {
                    int i_pos=0;
cout << "FileManager::writeDirectory2 : before find(userOptions->s_projectName)" << endl;
                    i_pos  = s_buffer.find(projectName);
 cout << "FileManager::writeDirectory2 : after find(userOptions->s_projectName)" << endl;
                    if(i_pos!=string::npos) {
                        int i_pos2 = s_buffer.rfind("#include", i_pos);
                        i_pos = s_buffer.find("\"", i_pos);
 cout << "FileManager::writeDirectory2 : before s_buffer.erase" << endl;
                        s_buffer.erase(s_buffer.begin()+i_pos2, s_buffer.begin()+i_pos+2);
 cout << "FileManager::writeDirectory2 : after s_buffer.erase : i_pos2"<<i_pos2 << endl;

                    }

                }


 cout << "FileManager::(*m_mIter).second) : " << (*m_mIter).second << endl;
		m_file.writeFile(s_buffer, s_fileName);
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
	//m_sRelativePathName = s_oldDirectory;
			}
			else {
				FileManager::m_sRelativePathName += "/" + string(dirp->d_name);
                                if(s_tempFullPathName.find(".cpp")!=string::npos ||
                                        s_tempFullPathName.find(".c")!=string::npos ||
                                        s_tempFullPathName.find(".h")!=string::npos ||
                                        s_tempFullPathName.find(".hpp")!=string::npos
                                  ) {
                                        m_file.loadFile(s_tempFullPathName, s_buffer);
                                        //m_setDisplayFiles.insert(string(dirp->d_name));
                                        m_vListOfBuffers.insert ( pair<string,string>(FileManager::m_sRelativePathName,s_buffer) );
                                        //m_sRelativePathName = s_oldDirectory;
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
                    s_FullFilePath.find(".hpp")!=string::npos
              ) {
                    m_file.loadFile(s_FullFilePath, s_buffer);
                    //m_setDisplayFiles.insert(string(dirp->d_name));
                    m_vListOfBuffers.insert ( pair<string,string>(s_parentDirectory, s_buffer) );
                    //closedir(dp);
                }
	}
	FileManager::m_sRelativePathName.clear();
	return 0;

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
