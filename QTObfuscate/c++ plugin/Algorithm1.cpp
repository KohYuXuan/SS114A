//#include "Engine.h"
#include "Algorithm1.h"
/*
void Algorithm1::loadFile(const string& fileName, string& v)
{
    v.clear();
    ifstream in(fileName.c_str());
	string words;
    int count=0;
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

void Algorithm1::loadFile(const string& fileName, set<string>& set_str)
{
    set_str.clear();
    ifstream in(fileName.c_str());
	string words;
    int count=0;
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


void Algorithm1::writeFile(const string& s_input, const string& fileName)
{
        //int j=v.size();
        ofstream out(fileName.c_str());
	//out<<"#include \"obsfun.h\""<<endl;
        for(int i=0;i<s_input.size();++i)
        {
            out << s_input[i];
            //if(i<j-1)       // no newline after last record output to file
            //{
                //out<<"\n";
            //}
            //out.flush();

        }
        out.close();
}
*/
void Algorithm1::addWordsToList(const string& str,vector<string>& v)
{
        locale loc;
        string s_temp;
        //bool s_found = false;
        //int numOfLetter;

	// find and skip header file
	int i_headerPos=0;
	int i_newlinePos=0;
	i_headerPos = str.rfind("#include");
	if(i_headerPos!=string::npos) {
		i_newlinePos = str.find("\n", i_headerPos);
	}












        for(int i=i_newlinePos; i<str.size(); ++i)
        {
                if(str[i] == '\"')
                        skipQuote(str, i, '\"');
                if(str[i] == '\'')
                        skipQuote(str, i, '\'');

                //while(str[i] != '\"') {
                        s_temp.clear();
                        while(isalnum(str[i],loc) || str[i]=='_')
                        {
                                s_temp.append(1,str[i]);
                                ++i;
                                //s_found = true;
                                //++numOfLetter;
                        }
                        if(!s_temp.empty())
                                v.push_back(s_temp);
                        //i += numOfLetter;
                        //if(i>=str.size()) break;
                        //if(!s_found)
                                //++i;
                        //s_found = false;
                //}++i;
                //while(str[i] != '\"') { // skip quoted string
                        //++i;
                //}
        }
        for(unsigned int i=0; i<v.size(); ++i)
        {
                cout << v[i] << endl;
        }
}

/////////////////////////////////////////////

char rand_alnum()
{
	
    char c;
    while (!std::isalpha(c = static_cast<char>(std::rand())))
        ;
    return c;
}


std::string Algorithm1::rand_alnum_str (std::string::size_type sz)
{	
    std::string s;
    s.reserve  (sz);
    generate_n (std::back_inserter(s), sz, rand_alnum);
    return s;
}
/////////////////////////////////////////////





void Algorithm1::mapping(map<string,string>& mymap,vector<string>& v)
{
	locale loc;
	//map<string,string> mymap;
	map<string,string>::iterator it;
	pair<map<string,string>::iterator,bool> ret;


	string str_random;
	string s_temp;
        for(unsigned int i=0; i<v.size(); ++i)
	{	
		s_temp.clear();
		s_temp = v[i];
		if(isalpha(s_temp[0],loc)) {
			str_random = rand_alnum_str(NUM_RANDOM);
			ret=mymap.insert (pair<string,string>(v[i],str_random) ); 
                                while (ret.second!=false)
				{
					ret=mymap.insert (pair<string,string>(v[i],str_random) ); 
				}
		}
	}

	for ( it=mymap.begin() ; it != mymap.end(); it++ )
		cout << (*it).first << " => " << (*it).second << endl;

}

void Algorithm1::modiftString(string& output, const string& str, const vector<string>& v,map<string,string>& mymap)
{	locale loc;
        size_t found=0;
	string s_temp, s_random;
	map<string,string>::iterator it;
	set<string>::iterator set_it;
	int i_vector=0;
	bool b_eof = false;
	output.clear();

if(v.empty()) {
	cout << "ReverseAlgorithm::vector is empty"  << endl;
cout << " shall inform user================================================================================="<<endl;
cout<<"reason may be mapping file not found=================================================="<<endl;
	return;
}














	for(int i=0; i<str.size(); ++i)
	{		

		if(i_vector < v.size() )//&& s_temp.compare(v[i_vector])!=0)	
			s_temp = v[i_vector];	

		//c_temp = str[i];
		if(found < str.size() && found != string::npos)
			found = str.find(s_temp,found);

		while(found!=i) {
			output.append(1,str[i]);
			++i;
			if(i >= str.size()) {
				b_eof = true;
				break;
			}
		}

		if(b_eof) break;
	
		if(isdigit(s_temp[0],loc)){ // if is digit, program will not run, so just add to buffer and not to obsfucate digit
			
			output.append(s_temp);
			found += s_temp.size();
			i = found-1;
			++i_vector;
		}
		if(found == i){
			


cout << "Algorithm1::modiftString : s_temp : " << s_temp << endl;
			it=mymap.find(s_temp);
                        if( it != mymap.end())
				s_random = (*it).second;//cout << (*it).second << endl;
//cout << s_temp << endl;
			output.append(s_random);
			found += s_temp.size();
			i = found-1;
			++i_vector;
		}
	}
	cout << output << endl;		
}



void Algorithm1::removeWordsFromList(set<string> set_input,vector<string>& vec)
{
	string s_temp;
	vector<string> v_output;
	set<string>::iterator set_it;
	for(int i=0; i<vec.size(); ++i)
	{
			s_temp = vec[i];
		if( (set_it=set_input.find(s_temp)) == set_input.end() ){
				//for ( set_it=set_keywords.begin() ; set_it != set_keywords.end(); set_it++ )
					//cout << "set : " << *set_it << endl;
			v_output.push_back(s_temp);
		}
	}
	vec.clear();
	vec.assign(v_output.begin(), v_output.end());

	for(int i=0; i<vec.size(); ++i)
	{
		cout << "vec : " << vec[i] << endl;
	}  
}

void Algorithm1::findWordsFromList(set<string>& set_input, vector<string>& v_input, vector<string>& v_output)
{
	if(set_input.empty() || v_input.empty())
		return;

	v_output.clear();
	string s_temp;
	//vector<string> v_output;
	set<string>::iterator set_it;
	for(int i=0; i<v_input.size(); ++i)
	{
			s_temp =  v_input[i];
		if( (set_it=set_input.find(s_temp)) != set_input.end() ){
				//for ( set_it=set_keywords.begin() ; set_it != set_keywords.end(); set_it++ )
					//cout << "set : " << *set_it << endl;
			v_output.push_back(s_temp);
		}
	}
	for(int i=0; i<v_output.size(); ++i)
	{
		cout << "findWordsFromList : " << v_output[i] << endl;
	} 
}

void Algorithm1::createHeaderFile(map<string,string>& mymap,string& s_output)
{
	s_output.clear();
	string s_header;
	string s_end;
	string s_define;
	s_define = "#define ";
	s_header = "#ifndef OBSFUN \n#define OBSFUN \n";
	s_end = "#endif \n";
	
	s_output = s_header;
	map<string,string>::iterator it;
	for ( it=mymap.begin() ; it != mymap.end(); it++ )
		s_output += s_define + (*it).second + " " + (*it).first + "\n";

	s_output += s_end;	

cout << "Algorithm1::createHeaderFile : " << s_output << endl;
}

void Algorithm1::skipQuote(const string& s_input, int& i_skip, char c_quote) // c_quote can be double or single quote
{
	for(int i=i_skip; i<s_input.size(); ++i)
	{	

		while(s_input[i] == c_quote) { // enter quoted string
			++i;
			while(s_input[i] != c_quote && s_input[i-1]!= '\\') { // skip quoted string
				++i;
			}
			++i;
			i_skip = i;
			return;
		}
	}
}

string Algorithm1::charToHex(char& c)
{
	stringstream ss;
	string s;

	ss << hex << (unsigned short) c;
	s = "\\x";
	//s[1] = 'x';
	s.append(ss.str());
cout << s << endl;
	return s;
}

void Algorithm1::stringToHex(const string& s,string& output)
{
	char c;
	//string hex;
	for(int i=0; i<s.size(); ++i)
	{
		c = s[i];
		if(c == '\\') { // escape character (i.e. \n)
			output.append(1,s[i]);
			output.append(1,s[i+1]);	
			++i;
			continue;	
		}
		if(c == '.') { // filename (i.e. word.txt)
			output.append(1,s[i]);
			continue;	
		}
		output.append( charToHex(c) );
	}
cout << "charToHex : " << output << endl;
	//return hex;
}

void Algorithm1::convertQuotedStringToHex(string& s_searchString)
{	
	size_t i_firstQuote;
	size_t i_secondQuote;
	size_t i_sizeOfQuotedString;
	vector<string> v_listOfQuotedString;
	string s_temp;
	string s_hex;
	int i_sizeToRepalce;
	int i_posToReplace;

	// find and skip header file
	int i_headerPos=0;
	int i_newlinePos=0;
	i_headerPos = s_searchString.rfind("#include");
	if(i_headerPos!=string::npos) {
		i_newlinePos = s_searchString.find("\n", i_headerPos);
	}

	//string s_searchString = "hellow world says \"abcdefg\" to \"gg\" world\n";
	string s_quotedString;
		i_firstQuote = s_searchString.find_first_of("\"", i_newlinePos);
		i_secondQuote = s_searchString.find_first_of("\"",i_firstQuote+1);

		while(i_firstQuote != string::npos && i_secondQuote != string::npos) {
			i_sizeOfQuotedString = i_secondQuote - i_firstQuote - 1;
			s_quotedString.assign(s_searchString, i_firstQuote+1, i_sizeOfQuotedString);	
			v_listOfQuotedString.push_back(s_quotedString);
			i_firstQuote = s_searchString.find_first_of("\"",i_secondQuote+1);
			i_secondQuote = s_searchString.find_first_of("\"",i_firstQuote+1);
		}
		for(int i=0; i<v_listOfQuotedString.size(); ++i) {
				//cout << v_listOfQuotedString[i] << endl;
			if(s_searchString.find(v_listOfQuotedString[i]) != string::npos) {
				s_temp = v_listOfQuotedString[i];
				i_posToReplace = s_searchString.find(s_temp);
				i_sizeToRepalce = s_temp.size();
				s_hex.clear();
				stringToHex(s_temp, s_hex);
				s_searchString.replace ( i_posToReplace, i_sizeToRepalce, s_hex, 0, s_hex.size() );
					
			}
		}
		cout << s_searchString << endl;

}

void Algorithm1::removeSpace( string& str_output )
{
	string::size_type i_pos = 0;
	bool b_spacesLeft = true;

	while( b_spacesLeft )
	{
		i_pos = str_output.find("  ");
		if( i_pos != string::npos )
		str_output.erase( i_pos, 1 );
		else
		b_spacesLeft = false;
	}

	b_spacesLeft = true;
	while( b_spacesLeft )
	{
		i_pos = str_output.find("\t");
		if( i_pos != string::npos )
		str_output.erase( i_pos,1 );
		else
		b_spacesLeft = false;
	}

	b_spacesLeft = true;
	while( b_spacesLeft )
	{
		i_pos = str_output.find("\n");
		if( i_pos != string::npos )
		str_output.erase( i_pos,1 );
		else
		b_spacesLeft = false;
	}


}

void Algorithm1::removeNewline( string& s_input, vector<string>& v_output)
{
	stringstream ss_stream;
	string s_temp;
	v_output.clear();


	ss_stream << s_input;

       while (!ss_stream.eof()){		
		getline(ss_stream, s_temp, '\n');
		if(!s_temp.empty())
			v_output.push_back(s_temp);
	}

	
}

void Algorithm1::formatWords(string& str_output)
{
	vector<string> v_input;
	string s_temp;
	//str_output.clear();
	
	removeNewline( str_output, v_input);
	str_output.clear();
        //str_output.append(HEADERFILE);
	for(int i=0; i<v_input.size(); ++i) {

		if(v_input[i].find("#") != string::npos) {
			if(v_input[i].find_last_of("\\") != string::npos) {
				while(v_input[i].find_last_of("\\") != string::npos) {
					str_output.append(v_input[i]);
					str_output.append("\n");
					++i;
					if(i>=v_input.size()) break;
				}
			}
			str_output.append(v_input[i]);
			str_output.append("\n");
		}
		else {
			s_temp = v_input[i];
			removeSpace(s_temp);
			str_output.append(s_temp);
			if(i < v_input.size()-1 && v_input[i+1].find("#") != string::npos)
				str_output.append("\n");
			else
				str_output.append(" ");
		}
	}
}



