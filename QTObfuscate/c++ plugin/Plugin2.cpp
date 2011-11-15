//#include "Engine.h"

#include "Plugin2.h"

void Plugin2::setUpFile()
{
//////////////////////////
//    toDo : reverse algorithm shall only allow one project at one time so set_filename.size shall not be more than one
//		- mapping shall be in choosen diretory to reverse algorithm?? how about different project? just combine the 				mapping? safe to assume that's no collision?? or user shall also choose where the mapping
//		- solution : shall combine different mapping, name of mapping file shall follow the format of be 				projectNameMAP.txt, purpose of following this format is so that different mapping files shall or can 				be stored in one location, do the same for header file
//			- different mapping file may be combine,advantage of combining different mapping->able to reverse 					algortihm on different project at one time but take note of possible collision ->when loading 					for mapping file, scan for *MAP.txt
//		-how to scan for *MAP.txt files?????????
//		- shall delete mapping file after reverse algorithm
//////////////////////////
        string s_fileName;
        set<string> set_fileName;
        set<string>::iterator m_setIter;
        //m_engine->getFileManager()->getTargetFile(set_fileName);
        m_engine->getmainwindow()->getfullPathNamesList(set_fileName);
 cout << " Plugin1::setUpFile() : before"<< endl;
	m_engine->getFileManager()->clearTargetFile();
        for ( m_setIter=set_fileName.begin() ; m_setIter != set_fileName.end(); m_setIter++ ) {
                s_fileName = *m_setIter;
 cout << " Plugin1::setUpFile() : " << s_fileName << endl;
                m_engine->getFileManager()->addTargetFile(s_fileName);
        }
}

void Plugin2::setUpMapping()
{
    string s_fileName = "mapping";
        string s_string;
cout << "Plugin2::setUpMapping() : "  << endl;

        set<string> set_fileName;
        set<string>::iterator m_setIter;
        //m_engine->getFileManager()->getTargetFile(set_fileName);
        m_engine->getmainwindow()->getfullPathNamesList(set_fileName);


	map<string,string> map_tempMapping;
	m_mapping.clear();
        for ( m_setIter=set_fileName.begin() ; m_setIter != set_fileName.end(); m_setIter++ ) {
		if((*m_setIter).find("MAP")!=string::npos) {
			//set_mappingName.insert(*m_setIter);
			s_fileName = *m_setIter;
			m_engine->getFileManager()->loadMappingFile(s_fileName, s_string);
			m_algorithm.setUpMapping(s_string, map_tempMapping);
			m_mapping.insert(map_tempMapping.begin(), map_tempMapping.end());
		}
	}



}

void Plugin2::algorithm() {

        string str;
        vector<string> v_listOfWords;
        vector<string> v_wordsToDefine;
        map<string,string> m_listOfBuffers;
        map<string,string> m_output;
        map<string,string> mymap;
        map<string,string> m_define;
        map<string,string>::iterator m_mIter;
        string output;
        string outputSecondPass;
        string s_headerFile;
        set<string> set_keywords;
        //set<string> set_wordsToFind;
        set<string> set_wordsNotTo;
        //set<int>::iterator it;

        setUpFile();
        setUpMapping();

        m_engine->getFileManager()->showTargetFile();
	m_engine->getFileManager()->clearFileBuffer();
        m_engine->getFileManager()->loadAllFileIntoBuffer();
        m_engine->getFileManager()->getListOfBuffers(m_listOfBuffers);

        //m_file.loadFile("target2.cpp", str);
        m_engine->getFileManager()->loadFile("keywords.txt", set_keywords);
        //m_file.loadFile("keywords.txt", set_wordsToFind);
        m_engine->getFileManager()->loadFile("notToWords.txt", set_wordsNotTo);
        for(unsigned int i=0; i<str.size(); ++i)
                cout << str[i];
        cout << endl;


for(m_mIter=m_listOfBuffers.begin(); m_mIter != m_listOfBuffers.end(); ++m_mIter) {

str.clear();
v_listOfWords.clear();

str = (*m_mIter).second;

m_algorithm.addWordsToList(str,v_listOfWords);
m_algorithm.removeWordsFromList(m_mapping ,v_listOfWords);
//m_algorithm.removeWordsFromList(set_wordsNotTo ,v_listOfWords);

        m_algorithm.convertQuotedStringToHex(str);


        //m_engine->getAlgorithm()->mapping(mymap,v_listOfWords);
       // m_engine->getAlgorithm()->mapping(m_define, v_wordsToDefine);

       // m_engine->getAlgorithm()->modiftString(output, str, v_listOfWords, m_mapping);
       // m_engine->getAlgorithm()->modiftString(, output, v_wordsToDefine, m_define);

m_algorithm.reverseModiftString(output, str, v_listOfWords, m_mapping);
//m_algorithm.reverseModiftString(outputSecondPass, output, v_wordsToDefine, m_mapping);
//string outputThirdPass;
m_algorithm.formatFile(output, outputSecondPass);






       // m_engine->getAlgorithm()->formatWords(outputThirdPass);
m_output.insert ( pair<string,string>((*m_mIter).first, outputSecondPass ));
}

for ( m_mIter=m_output.begin() ; m_mIter != m_output.end(); m_mIter++ ) {
    cout << "m_output: " << (*m_mIter).first << endl;
}



string directoryName;
//string headerFileName;
string mappingFileName;
string folderName;
string projectName;
directoryName = m_userOptions->s_saveToDirectory;
projectName = m_userOptions->s_projectName;             //= "/obsfun";
folderName = "/" + projectName;
directoryName.append(folderName);

cout << "plugin2: before s_headerFileName"  << endl;
m_userOptions->s_headerFileName.clear();		//"/obsfun.h";


cout << "plugin2: after s_headerFileName"  << endl;
//headerFileName.insert(0, directoryName);
//headerFileName.append(m_userOptions->s_headerFileName);
//m_userOptions->s_mappingFileName = "/mapping";
//mappingFileName.insert(0, directoryName);
//mappingFileName.append(m_userOptions->s_mappingFileName);

m_engine->getFileManager()->writeDirectory(directoryName, m_output, projectName);
        //m_file.writeFile(outputSecondPass, "output.cpp");
       // m_engine->getAlgorithm()->createHeaderFile(m_define, s_headerFile);    // take note of multiple incldue of header, #ifdef #define....#endif
       // m_engine->getFileManager()->writeFile(s_headerFile, headerFileName);





}

