#include "File.h"



void File::loadFile(const string& fileName, string& v_output)
{
    v_output.clear();
    ifstream in(fileName.c_str());
	string words;
    if(in.is_open()) {
        while (!in.eof()){
            getline(in,words);
            if(!words.empty())
            {
                v_output.append(words);
                v_output.append(1,'\n');
            }
        }
    }else
        {
            cout<<"Error opening file!\n";
        }
	in.close();
}

void File::loadFile(const string& fileName, vector<string>& vec_str)
{
    vec_str.clear();
    ifstream in(fileName.c_str());
        string words;
    if(in.is_open()) {
        while (!in.eof()){
            getline(in,words);
            if(!words.empty())
            {
                vec_str.push_back(words);
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
       ofstream out(fileName.c_str());
       out << s_input;
       out.close();
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
