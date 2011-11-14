#include "File.h"
#include <algorithm>


void print()
{
	cout << "Hello" << endl;
}

void File::loadFile(const string& fileName, string& v)
{
    v.clear();
    ifstream in(fileName.c_str());
	string words;
    if(in.is_open()) {
        while (!in.eof()){
            getline(in,words);
            if(!words.empty())
            {
                v.append(words);
		v.append(1,'\n');
            }
        }
    }else
        {
            cout<<"Error opening file!\n";
        }
	in.close();
}

void File::loadFile(const string& fileName, set<string>& set_str)
{
    set_str.clear();
    ifstream in(fileName.c_str());
	string words;
    if(in.is_open()) {
        while (!in.eof()){
            getline(in,words);
            if(!words.empty())
            {
                set_str.insert(words);
		//set_str.append(1,'\n');
            }
        }
    }else
        {
            cout<<"Error opening file!\n";
        }
	in.close();
}


void File::writeFile(const string& s_input, const string& fileName)
{

 cout << "FFile::writeFile : s_input: " << s_input <<endl;
        //int j=v.size();
       ofstream out(fileName.c_str());
	//out<<"#include \"obsfun.h\""<<endl;
        //for( int i=0;i<s_input.size();++i)
       // {
            out << s_input;
            //if(i<j-1)       // no newline after last record output to file
            //{
                //out<<"\n";
            //}
            //out.flush();

       // }
        out.close();

/*
        wstring temp(s_input.length(),L' ');
        copy(s_input.begin(),s_input.end(),temp.begin());
        wcout << "FFile::writeFile : wstring  : "<< temp << endl;

        std::locale   defaultLocale(""); // from machine config
        wfstream wout;
        wout.imbue(defaultLocale);        // imbue must be done before opening
                                         // otherwise it is ignored.
        wout.open(fileName.c_str(), ios::in | ios::out | ios::trunc );


        if (!wout)
        {
            std::cout << "Failed to open file\n";
        }
       wout.write(temp.c_str(), temp.size());
        wout.flush();    // flush() happens automatically when the file is closed
        wout.close();
*/
 //cout << "FFile::writeFile : 2 :"<< s_input << endl;
}

void File::writeMappingToFile(map<string,string>& m_input, const string& fileName)
{
        map<string,string>::iterator m_iter;
        ofstream out(fileName.c_str());

        for ( m_iter=m_input.begin() ; m_iter != m_input.end(); m_iter++ )
            out << (*m_iter).second << ";" <<(*m_iter).first << endl;

        out.close();
}

void File::loadMappingFile(const string& fileName, string& s_output)
{
    s_output.clear();
    ifstream in(fileName.c_str());
    string words;
    if(in.is_open()) {
        while (!in.eof()){
            getline(in,words);
            if(!words.empty())
            {
                s_output.append(words);
                s_output.append(1,';');

            }
        }
    }else
        {
            cout<<"Error opening file!\n";
        }
        in.close();

}


