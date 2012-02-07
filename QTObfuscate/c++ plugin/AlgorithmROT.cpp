
#include "AlgorithmROT.h"

void Algorithm1::reverseFormatFile(const string& s_input, string& s_output)
{
        string s_tabToAppend;
	   	int i_tabToAppend = 0;
		s_output.clear();

                for(int i=0; i<s_input.size();++i) {

                        s_tabToAppend.clear();

                        for(int k=0; k<i_tabToAppend; ++k) {
                                s_tabToAppend.append("\t");
                        }
                        if(s_input[i] == '{') {
                                s_output.append("\n");
                                s_output.append(s_tabToAppend);
                                s_output.append("{");
                                s_output.append("\n");
                                s_output.append("\t");
                                s_output.append(s_tabToAppend);
                                ++i_tabToAppend;
                        }
                        else if(s_input[i] == '}') {
                                s_output.append("\n");
				if(!s_tabToAppend.empty())
	                                s_tabToAppend.erase(s_tabToAppend.begin());
				if(!s_tabToAppend.empty())
                               		 s_output.append(s_tabToAppend);

                                s_output.append("}");
                                s_output.append("\n");
                               // s_output.append("\t");
                                --i_tabToAppend;
                        }
                        else if(s_input[i] == ';') {
                                s_output.append(";");
                                s_output.append("\n");
                                s_output.append(s_tabToAppend);
                        }
                        else {
                                s_output.append(1, s_input[i]);
                        }
                }
                cout << s_output << endl;
}

char Algorithm1::rot13(char x)
{
        char rotated;

        if(('a' <= x && 'n' > x) || ('A' <= x && 'N' > x)) // if the letter is within the upper 13 characters
                rotated = x + 13; // add 13
        else
                rotated = x - 13; // otherwise, subtract 13

        return rotated; // return the rotated text
}

char rand_alnum()
{
    char c;
    while (!std::isalpha(c = static_cast<char>(std::rand())));
    return c;
}

std::string Algorithm1::rand_alnum_str (std::string::size_type sz)
{	
    std::string s;
    s.reserve  (sz);
    generate_n (std::back_inserter(s), sz, rand_alnum);
    return s;
}

void Algorithm1::addWordsToList(string& s_input,vector<string>& v_targetWords)
{
        locale loc;
        string s_word;

        for(int i=0; i<s_input.size(); ++i)
        {
                if(s_input[i] == '\"')
                        skipQuote(s_input, i, '\"');
                if(s_input[i] == '\'')
                        skipQuote(s_input, i, '\'');

		if(s_input[i] == '#' && s_input[i+1] == 'i' && 
		   s_input[i+2] == 'n' && s_input[i+3] == 'c' ) // check for #include
				skipHeader(s_input, i, '>');

		if(s_input[i] == '/' && s_input[i+1] == '/')
			removeComment(s_input, i);

                s_word.clear();

                while(isalnum(s_input[i],loc) || s_input[i]=='_' )
                {
                	s_word.append(1,s_input[i]);
                	++i;
                }

                if(!s_word.empty())
                	v_targetWords.push_back(s_word);
        }
}

void Algorithm1::mapping(map<string,string>& m_input,vector<string>& v_targetWords)
{
	locale loc;
	string s_randomString;
	string s_word;
	pair<map<string,string>::iterator,bool> pair_iter;

        for(unsigned int i=0; i<v_targetWords.size(); ++i)
	{	
		s_word.clear();
		s_word = v_targetWords[i];
		if(isalpha(s_word[0],loc)) {
			s_randomString.clear();
			for (int k=0; k<s_word.size(); ++k) {
				if (isalpha(s_word[k]))
					s_randomString += rot13(s_word[k]);
				else
					s_randomString += s_word[k];
			}
			pair_iter=m_input.insert (pair<string,string>(v_targetWords[i],s_randomString) ); 

                        while (pair_iter.second!=false)
			{
				pair_iter=m_input.insert (pair<string,string>(v_targetWords[i],s_randomString) ); 
			}
		}
	}
}

void Algorithm1::modiftString(string& output, const string& s_input, const vector<string>& v_targetWords,map<string,string>& m_input)
{	
	locale loc;
        size_t found=0;
	string s_word, s_randomString;
	int i_indexOfTargetWords=0, i_endOfQuote=0;
	bool b_eof = false; // end of file
	map<string,string>::iterator m_iter;
	output.clear();

	if(v_targetWords.empty()) {
		return;
	}

	for(int i=0; i<s_input.size(); ++i)
	{		

		if(i_indexOfTargetWords < v_targetWords.size() )	
			s_word = v_targetWords[i_indexOfTargetWords];	

		if(found < s_input.size() && found != string::npos) {
			found = s_input.find(s_word,found);
			if(s_input[found-1] == '\"')
				found = s_input.find(s_word,found+1);
		}


		while(found!=i) {
			output.append(1,s_input[i]);
			++i;
			
			if(s_input[i] == '#' && s_input[i+1] == 'i' && 
		   	   s_input[i+2] == 'n' && s_input[i+3] == 'c' ) { // check for #include
				skipHeader(s_input, i_endOfQuote, '>');

				if(i_endOfQuote > found) {
					found = s_input.find(s_word,found);
					if(s_input[found-1] == '\"' || s_input[found-1] == '<')
						found = s_input.find(s_word,found+1);		
				}
			}

		        if(s_input[i] == '\"') {
		                skipQuote(s_input, i_endOfQuote, '\"');
				if(i_endOfQuote > found) {
					found = s_input.find(s_word,found);
					if(s_input[found-1] == '\"')
						found = s_input.find(s_word,found+1);		
				}

			}

		        if(s_input[i] == '\'') {
		                skipQuote(s_input, i_endOfQuote, '\'');
				if(i_endOfQuote > found) {
					found = s_input.find(s_word,found);
					if(s_input[found-1] == '\"')
						found = s_input.find(s_word,found+1);		
				}
			}

			if(i >= s_input.size()) {
				b_eof = true;
				break;
			}
		}

		if(b_eof) break;
	
		if(isdigit(s_word[0],loc)){ 
			output.append(s_word);
			found += s_word.size();
			i = found-1;
			++i_indexOfTargetWords;
		}

		if(found == i){
			m_iter=m_input.find(s_word);
                        if( m_iter != m_input.end())
				s_randomString = (*m_iter).second;

			output.append(s_randomString);
			found += s_word.size();
			i = found-1;
			++i_indexOfTargetWords;
		}
	}	
}

void Algorithm1::removeWordsFromList(set<string> set_input,vector<string>& v_targetWords)
{
	string s_word;
	vector<string> v_tempOutput;
	set<string>::iterator set_iter;

	for(int i=0; i<v_targetWords.size(); ++i)
	{
		s_word = v_targetWords[i];

		if( (set_iter=set_input.find(s_word)) == set_input.end() ){
			v_tempOutput.push_back(s_word);
		}
	}

	v_targetWords.clear();
	v_targetWords.assign(v_tempOutput.begin(), v_tempOutput.end());
}

void Algorithm1::findWordsFromList(set<string>& set_input, vector<string>& v_input, vector<string>& v_output)
{
	if(set_input.empty() || v_input.empty())
		return;

	v_output.clear();
	string s_word;
	set<string>::iterator set_iter;

	for(int i=0; i<v_input.size(); ++i)
	{
		s_word =  v_input[i];

		if( (set_iter=set_input.find(s_word)) != set_input.end() ){
			v_output.push_back(s_word);
		}
	}
}

void Algorithm1::createHeaderFile(map<string,string>& m_input, string& s_output)
{
	s_output.clear();
	string s_header;
	string s_end;
	string s_define;
	map<string,string>::iterator m_iter;

	s_define = "#define ";
	s_header = "#ifndef OBSFUN \n#define OBSFUN \n";
	s_end = "#endif \n";
	s_output = s_header;

	for ( m_iter=m_input.begin() ; m_iter != m_input.end(); m_iter++ )
		s_output += s_define + (*m_iter).second + " " + (*m_iter).first + "\n";

	s_output += s_end;	
}

void Algorithm1::convertQuotedStringToHex(string& s_searchString)
{	
	size_t i_firstQuote=0;
	size_t i_secondQuote;
	size_t i_sizeOfQuotedString;
	vector<string> v_listOfQuotedString;
	string s_temp;
	string s_hex;
	int i_sizeToRepalce;
	int i_posToReplace;

/*
	// find and skip header file
	int i_headerPos=0;
	int i_newlinePos=0;
	i_headerPos = s_searchString.rfind("#include");
	if(i_headerPos!=string::npos) {
		i_newlinePos = s_searchString.find("\n", i_headerPos);
	}
*/

		string s_quotedString;
		i_firstQuote = s_searchString.find_first_of("\"", i_firstQuote);
		i_secondQuote = s_searchString.find_first_of("\"",i_firstQuote+1);
		while(s_searchString[i_secondQuote-1] == '\\') {
			i_secondQuote = s_searchString.find_first_of("\"",i_secondQuote+1);
		}

		while(i_firstQuote != string::npos && i_secondQuote != string::npos) {
			i_sizeOfQuotedString = i_secondQuote - i_firstQuote - 1;
			s_quotedString.assign(s_searchString, i_firstQuote+1, i_sizeOfQuotedString);
			if(s_quotedString.find(".h") == string::npos)
				v_listOfQuotedString.push_back(s_quotedString);
			i_firstQuote = s_searchString.find_first_of("\"",i_secondQuote+1);
			i_secondQuote = s_searchString.find_first_of("\"",i_firstQuote+1);
			while(s_searchString[i_secondQuote-1] == '\\') {
				i_secondQuote = s_searchString.find_first_of("\"",i_secondQuote+1);
			}
		}
		for(int i=0; i<v_listOfQuotedString.size(); ++i) {
			if(s_searchString.find(v_listOfQuotedString[i]) != string::npos) {
				s_temp = v_listOfQuotedString[i];
				i_posToReplace = s_searchString.find(s_temp);
				i_sizeToRepalce = s_temp.size();
				s_hex.clear();
				stringToHex(s_temp, s_hex);
				s_searchString.replace ( i_posToReplace, i_sizeToRepalce, s_hex, 0, s_hex.size() );
			}
		}
}

void Algorithm1::formatWords(string& str_output)
{
	vector<string> v_input;
	string s_temp;
	
	removeNewline( str_output, v_input);
	str_output.clear();

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

void Algorithm1::skipQuote(const string& s_input, int& i_skip, char c_quote) 
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
	s.append(ss.str());
	return s;
}

void Algorithm1::stringToHex(const string& s,string& output)
{
	char c;
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

void Algorithm1::skipHeader(const string& s_input, int& i_skip, char c_bracket) 
{
	int i = i_skip;

	while(s_input[i] == '#' && s_input[i+1] == 'i' && 
		   	   s_input[i+2] == 'n' && s_input[i+3] == 'c' ) // check for #include
	{
		while (s_input[i] != '>')
		{
			 if(s_input[i] == '\"') {
		                skipQuote(s_input, i, '\"');
				i_skip = i;
				break;
			}

			++i;
		}

		if(s_input[i] == '>')
			++i;

		i_skip = i;

	}
}

void Algorithm1::removeComment(string& s_input, int& i_skip) 
{
	int i = i_skip, i_toRemove = 0;
	
	if(s_input[i] == '/' && s_input[i+1] == '/')
	{
		i_toRemove = s_input.find("\n",i);
		s_input.erase(i, i_toRemove);

		//i_skip = i;
	}
}



