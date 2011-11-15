
#include "Plugin1.h"



 void Plugin1::setUpFile()
{
        string s_fileName;
	set<string> set_fileName;
	set<string>::iterator m_setIter;
	//m_engine->getFileManager()->getTargetFile(set_fileName);
 cout << " Plugin1::setUpFile() : before"<< endl;
        m_engine->getmainwindow()->getfullPathNamesList(set_fileName);
//set_fileName.insert("/home/koh/Desktop/QTObfuscate/target2.cpp");

	m_engine->getFileManager()->clearTargetFile();
	for ( m_setIter=set_fileName.begin() ; m_setIter != set_fileName.end(); m_setIter++ ) {
                s_fileName = *m_setIter;
 cout << " Plugin1::setUpFile() : " << s_fileName << endl;
                m_engine->getFileManager()->addTargetFile(s_fileName);
	}
}

void Plugin1::algorithm() {
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

cout << "Plugin1::algorithm()" << endl;

//m_engine = g_engine;
//m_engine = new Engine;
//m_engine = g_engine;

/*
string fileName;
set<string> set_fileName;
set<string>::iterator m_setIter;
m_engine->getFileManager()->getTargetFile(set_fileName);

for ( m_setIter=set_fileName.begin() ; m_setIter != set_fileName.end(); m_setIter++ ) {
	fileName = *m_setIter;
	m_engine->getFileManager()->addTargetFile(fileName);
}
*/

        setUpFile();


cout << "Plugin1::setUpFile() : after" << endl;

//m_fileManager.addTargetFile("target2.cpp");
//m_fileManager.addTargetFile("main.cpp");
m_engine->getFileManager()->showTargetFile();
m_engine->getFileManager()->clearFileBuffer();
m_engine->getFileManager()->loadAllFileIntoBuffer();
m_engine->getFileManager()->getListOfBuffers(m_listOfBuffers);

cout << "Plugin1::getListOfBuffers() : after" << endl;

	//m_file.loadFile("target2.cpp", str);
	m_engine->getFileManager()->loadFile("keywords.txt", set_keywords);
	//m_file.loadFile("keywords.txt", set_wordsToFind);
	m_engine->getFileManager()->loadFile("notToWords.txt", set_wordsNotTo);	
        for(unsigned int i=0; i<str.size(); ++i)
		cout << str[i];
	cout << endl;
	//if quoted, do not addWordsToList
	//take note of qutoed string inside a quoted string

for(m_mIter=m_listOfBuffers.begin(); m_mIter != m_listOfBuffers.end(); ++m_mIter) {

str.clear();
v_listOfWords.clear();

str = (*m_mIter).second;
	// look for single quoted char
	m_algorithm.addWordsToList(str,v_listOfWords);

	m_algorithm.findWordsFromList(set_keywords,v_listOfWords, v_wordsToDefine);
//m_file.findNumFromList // check !isalpha && !='.'	
//m_file.findFloatFromList // float can be in .0456

	m_algorithm.removeWordsFromList(set_keywords ,v_listOfWords);
	m_algorithm.removeWordsFromList(set_wordsNotTo ,v_listOfWords);
//m_file.removeDigitFromList
	m_algorithm.convertQuotedStringToHex(str);


	m_algorithm.mapping(mymap,v_listOfWords);
	m_algorithm.mapping(m_define, v_wordsToDefine);


//m_file.convertDigitToHex
//m_file.convertFloatToHex


        m_algorithm.modiftString(output, str, v_listOfWords, mymap);
        m_algorithm.modiftString(outputSecondPass, output, v_wordsToDefine, m_define);


	m_algorithm.formatWords(outputSecondPass);
m_output.insert ( pair<string,string>((*m_mIter).first, outputSecondPass ));
}




for ( m_mIter=m_output.begin() ; m_mIter != m_output.end(); m_mIter++ ) {
    cout << "m_output: " << (*m_mIter).first << endl;
}




string directoryName;
string headerPathName;
string mappingFileName;
string folderName;
string projectName;
string adminName;
string adminPathName;
adminName = ADMIN_FILE_NAME;


directoryName = m_userOptions->s_saveToDirectory;
projectName = m_userOptions->s_projectName;             //= "/obsfun";
folderName = "/" + projectName;
directoryName.append(folderName);
m_userOptions->s_headerFileName = projectName + ".h";       //"/obsfun.h";
//headerPathName.insert(0, directoryName);
//headerPathName.append(adminName);
/*
adminPathName = m_userOptions->s_adminFileName;
adminPathName.append(adminName);
*/
//cout << "after append(adminName)" << headerPathName << endl;
//adminPathName = headerPathName;

//headerPathName.append(m_userOptions->s_headerFileName);

//cout << "after append(m_userOptions->s_headerFileName);" << headerPathName << endl;




adminPathName = m_userOptions->s_adminFileName;
adminPathName += "/" + projectName + ADMIN_FILE_NAME + "/"; 



headerPathName = adminPathName + m_userOptions->s_headerFileName;




m_userOptions->s_mappingFileName = "/" + projectName + "MAP";         //"/mapping";
mappingFileName =  adminPathName + m_userOptions->s_mappingFileName;
/*
mappingFileName.insert(0, directoryName);
mappingFileName.append(adminName);
mappingFileName.append(m_userOptions->s_mappingFileName);
*/
m_engine->getFileManager()->writeDirectory(directoryName, m_output, projectName, m_userOptions->s_headerFileName, adminPathName);
	//m_file.writeFile(outputSecondPass, "output.cpp");
	m_algorithm.createHeaderFile(m_define, s_headerFile);    // take note of multiple incldue of header, #ifdef #define....#endif
        m_engine->getFileManager()->writeFile(s_headerFile, headerPathName);


        map<string,string> m_combinedMapping;
        m_combinedMapping = m_define;
        m_combinedMapping.insert(mymap.begin(),mymap.end());
        m_engine->getFileManager()->writeMappingToFile(m_combinedMapping, mappingFileName);

// take note of openFile("word.txt")  ,   if change word.txt, cannot open file
//if change filename, need to change #include "header.h" also
//in situation like #define abc 4(scan for them), then add #define random_string abc on top
//option to choose if want to obfucate int , float






//delete m_engine;







}




