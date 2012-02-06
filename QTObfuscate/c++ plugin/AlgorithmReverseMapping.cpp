
#include "AlgorithmReverseMapping.h"

// does not work if header files contain "#define" in double quote
// does not work if header files contain "#include" in double quote
// todo : change PluginReverseDefine it's header file path

bool AlgorithmDefine::algorithmMap(string& s_input, string& s_sourceOutput, map<string,string>& m_mappingReverseDefine, bool b_rot)
{
    m_listOfBlocks.clear();

    int i_startQuote=0, i_endQuote=0, i_pos=0;

    bool b_addNewLine = false;

    string s_chars, s_rand, s_output
            , s_randName,s_block;


    cout << "inside mapstring :" << endl;
        for(m_iter=m_mappingReverseDefine.begin(); m_iter != m_mappingReverseDefine.end(); ++m_iter)
            cout << (*m_iter).first << " : " <<  (*m_iter).second << endl;



    while (i_pos < s_input.size()) {
        s_output.clear();


        if (s_input[i_pos] == '\"'){
            i_startQuote = i_pos;
            skipQuote(s_input, i_pos, '\"');
            i_endQuote = i_pos;
            s_output.assign(s_input, i_startQuote, i_endQuote-i_startQuote);
            s_block =  " " + s_output + " ";
            m_listOfBlocks.push_back(s_block);
            continue;
        }

        if (s_input[i_pos] == '\'') {
            i_startQuote = i_pos;
            skipQuote(s_input, i_pos, '\'');
            i_endQuote = i_pos;
            s_output.assign(s_input, i_startQuote, i_endQuote-i_startQuote);
            s_block =  " " + s_output + " ";
            m_listOfBlocks.push_back(s_block);
            continue;
        }

        if(s_input[i_pos] == '#' && s_input[i_pos+1] == 'i' &&
            s_input[i_pos+2] == 'f' && s_input[i_pos+3] == 'd' &&
            s_input[i_pos+4] == 'e' && s_input[i_pos+5] == 'f'){ // check for #ifdef
                int i_tempPos = i_pos;
                string s_tempBlock;
                i_pos = s_input.find("#endif",i_pos);
                i_pos = s_input.find("\n",i_pos);

                s_tempBlock.assign(s_input, i_tempPos, i_pos-i_tempPos);
                s_block = "\n" + s_tempBlock + "\n";
                m_listOfBlocks.push_back(s_block);
                continue;
        }

        if(s_input[i_pos] == '#'){ // check for #define, #ifdef
                int i_tempPos = i_pos;
                string s_tempBlock;
                i_pos = s_input.find("\n",i_pos);

                s_tempBlock.assign(s_input, i_tempPos, i_pos-i_tempPos);
                s_block = "\n" + s_tempBlock + "\n";
                m_listOfBlocks.push_back(s_block);
                continue;
        }

        if(s_input[i_pos] == '/' && s_input[i_pos+1] == '/'){
                removeComment(s_input, i_pos);
                continue;
        }

        if(s_input[i_pos] == '/' && s_input[i_pos+1] == '*'){
                removeComment(s_input, i_pos);
                continue;
        }

        if(!isalnum(s_input[i_pos]) ){
                s_output = s_input[i_pos];
                m_listOfBlocks.push_back(s_output);
                ++i_pos;
                continue;
        }

        s_output.clear();
        s_chars.clear();

        while ( i_pos < s_input.size()) {
                if (!isalnum(s_input[i_pos]) && s_input[i_pos] != '_') {
                    --i_pos;
                    break;
                }

                s_chars.push_back(s_input[i_pos]);
                ++i_pos;
        }

    cout << "b_rot : " << b_rot << endl;
            mapString(s_chars, m_mappingReverseDefine,b_rot);
            ++i_pos;
        }

        string s_formated;

        convertToString(m_listOfBlocks,s_formated);

        s_sourceOutput = s_formated;
}

void AlgorithmDefine::mapString(string& s_input, map<string,string>& m_mappingReverseDefine, bool b_rot)
{
	int i_num;
	string s_name, s_name2, s_argument, s_randName, s_headerFile;
	vector<int> v_charPos;
	vector<string> v_listOfArgument;

	if (s_input.empty())
		return;

    m_iter = m_mappingReverseDefine.find(s_input);

cout << "         s_input : " << s_input << endl;
    if (m_iter != m_mappingReverseDefine.end()) {
        s_name = (*m_iter).second;

cout << "s_name : " << s_name << endl;
        if(b_rot)
            s_name = rot13(s_name);

        m_listOfBlocks.push_back( s_name );
    }
    else {
        m_listOfBlocks.push_back(s_input);
    }
}

void AlgorithmDefine::skipQuote(const string& s_input, int& i_skip, char c_quote)
{
	for(int i=i_skip; i<s_input.size(); ++i)
	{
		while(s_input[i] == c_quote) { // enter quoted string
			++i;
			while(s_input[i] != c_quote || s_input[i-1]== '\\' && s_input[i-2] != '\\') { // skip quoted string
				++i;
			}
			++i;
			i_skip = i;
			return;
		}
	}
}

void AlgorithmDefine::format(string& s_input, string& s_output)
{
        formatWords(s_input);

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

void AlgorithmDefine::convertToString(vector<string>& v_input, string& s_output)
{
    for (int j=0; j<v_input.size(); ++j) {
            s_output += v_input[j];
    }
    return;
}

void AlgorithmDefine::removeComment(string& s_input, int& i_skip)
{
	int i = i_skip, i_toRemove = 0;

	if(s_input[i] == '/' && s_input[i+1] == '/')
	{
		i_toRemove = s_input.find("\n",i);
		s_input.erase(i, i_toRemove-i);

		//i_skip = i;
	}
	else if(s_input[i] == '/' && s_input[i+1] == '*')
	{
		i_toRemove = s_input.find("*/",i);
		s_input.erase(i, i_toRemove-i+2);

	}
}

void AlgorithmDefine::setListOfKeywords(set<string>& set_listOfKeywords)
{
    m_listOfKeywords = set_listOfKeywords;
}

void AlgorithmDefine::clearAlgorithmBuffer()
{
    m_listOfDefine.clear();
    m_listOfInclude.clear();
    m_listOfBlocks.clear();
    m_duplicateCheck.clear();
    m_listOfKeywords.clear();
    m_setDuplicateMappingCheck.clear();
    m_mappingOutput.clear();
}

void AlgorithmDefine::getMappingList(map<string, string>& m_output)
{
    m_output.insert( m_mappingOutput.begin(), m_mappingOutput.end() );
}

string AlgorithmDefine::rot13(string s_input)
{
    char rotated, original;
    string s_output;
    for (int i=0; i<s_input.size(); ++i) {
            original = s_input[i];
        if (!isalpha(original) )
            rotated = original;
        else if(('a' <= original && 'n' > original) || ('A' <= original && 'N' > original))
            rotated = original + 13;
        else
            rotated = original - 13;

        s_output.push_back(rotated);
    }
        return s_output;
}

void AlgorithmDefine::getMappingReverse(string& s_input, map<string,string>& m_output)
{
    string s_alpha = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string s_symbol = "(),";
    string s_newLine = "\n\r";

    map<string,vector<string> > map_pos_arg;
    map<string,vector<string> > map_rand_arg;
    map<string,vector<string> > map_orig_arg;
    map<string,map<string,string> > map_function_arg;
    map<string,string> map_rand_orig_arg;
    map<string,string> map_rand_orig_func;
    map<string,string> map_rand_orig_Name;

    int i_startPos=0, i_openBrackets, i_numSpaces,
        i_endPos, i_endBracketPos, i_newLinePos,
        i_quotePos=0, i_singleQuotePos;
    string s_origFunc, s_randArg, s_posArg, s_origFunc2, s_randFunc, s_origArg;
    string s_randName, s_origName;
    vector<string> v_randArg, v_posArg, v_origArg;


    i_startPos = s_input.find("XENUS_ENGINE");
    i_startPos = s_input.find("XENUS_ENGINE", i_startPos+1);
    if (i_startPos == string::npos)
        return;

    while (s_input.find("define", i_startPos+1) != string::npos) {
            i_startPos = s_input.find("define", i_startPos+1);
            i_openBrackets = s_input.find("(", i_startPos);
            i_numSpaces = s_input.find(" ", i_startPos);
            if(i_numSpaces != string::npos)
                i_numSpaces = s_input.find(" ", i_numSpaces+1);
            i_quotePos =  s_input.find("\"", i_startPos);
            i_singleQuotePos =  s_input.find("\'", i_startPos);

            if(i_startPos == string::npos)
                return;
            if(i_numSpaces == string::npos)
                i_numSpaces = s_input.size();
            if(i_openBrackets == string::npos)
                i_openBrackets = s_input.size();
            if(i_numSpaces == string::npos)
                i_numSpaces = s_input.size();
            if(i_quotePos == string::npos)
                i_quotePos = s_input.size();
            if(i_singleQuotePos == string::npos)
                i_singleQuotePos = s_input.size();

            cout << "testing : "  << endl;

            if (s_symbol.find_first_of(s_input[i_numSpaces+2]) != string::npos
                && s_input[i_numSpaces+1] != '\"'
                && s_input[i_numSpaces+1] != '\'' ) {
                i_startPos = s_input.find(" ", i_startPos);
                i_startPos = s_input.find_first_of(s_alpha, i_startPos);
                i_endPos =  s_input.find_first_not_of(s_alpha, i_startPos+1);
                s_randName.assign(s_input, i_startPos, i_endPos - i_startPos);

                s_origName = s_input[i_numSpaces+2];
                m_output.insert(pair<string,string>(s_randName, s_origName));

            }

            else if (i_openBrackets < i_numSpaces
                && i_openBrackets < i_quotePos  && i_openBrackets < i_singleQuotePos ){
                i_startPos =  s_input.find(" ", i_startPos);
                i_endPos =  s_input.find("(", i_startPos);
                s_origFunc.assign(s_input, i_startPos+1, i_endPos - i_startPos-1);
            cout << " s_origFunc: " << s_origFunc << endl;

                // find random argument list
                i_endBracketPos = s_input.find(")", i_endPos);
                v_randArg.clear();
                while (i_endPos < i_endBracketPos && i_endPos != string::npos) {
                    i_startPos = i_endPos;
                    i_endPos =  s_input.find(",", i_startPos+1);
                    if (i_endPos == string::npos || i_endPos > i_endBracketPos)
                        i_endPos = i_endBracketPos;
                    s_randArg.assign(s_input, i_startPos+1, i_endPos - i_startPos-1);
                    v_randArg.push_back(s_randArg);
            //cout << " s_randArg: " << s_randArg << endl;
                }

                // find position of arguments
                i_newLinePos = s_input.find_first_of(s_newLine, i_startPos);
                if (i_newLinePos == string::npos)
                    i_newLinePos = s_input.size();
                i_startPos = s_input.find_first_of(s_alpha, i_endPos);
                i_endPos = i_startPos;
                //i_endPos =  s_input.find_first_not_of(s_alpha, i_startPos);
                v_posArg.clear();
                while (i_endPos < i_newLinePos && i_endPos != string::npos) {
                            i_startPos = s_input.find_first_of(s_alpha, i_endPos);
                            i_endPos =  s_input.find_first_not_of(s_alpha, i_startPos+1);
                            if (i_startPos == string::npos) {
                                i_startPos = i_newLinePos;
                                 break;
                            }
                            if (i_endPos == string::npos)
                                i_endPos = i_newLinePos;
                            s_posArg.assign(s_input, i_startPos, i_endPos - i_startPos);
                            v_posArg.push_back(s_posArg);
                }
                map_pos_arg.insert( pair<string,vector<string> >(s_origFunc, v_posArg) );
                map_rand_arg.insert( pair<string,vector<string> >(s_origFunc, v_randArg) );
            }

            else if (i_openBrackets > i_numSpaces
                     && i_openBrackets < i_quotePos  && i_openBrackets < i_singleQuotePos )
            {
                i_startPos =  s_input.find(" ", i_startPos);
                i_endPos =  s_input.find(" ", i_startPos+1);
                s_randFunc.assign(s_input, i_startPos+1, i_endPos - i_startPos-1);
                cout << " s_randFunc: " << s_randFunc << endl;
                // find original function
                i_startPos = i_endPos;
                i_endPos =  s_input.find("(", i_startPos);
                s_origFunc2.assign(s_input, i_startPos+1, i_endPos - i_startPos-1);
                cout << " s_origFunc2: " << s_origFunc2 << endl;
                // find original arguments
                i_endBracketPos = s_input.find(")", i_endPos);
                v_origArg.clear();
                while (i_endPos < i_endBracketPos && i_endPos != string::npos) {
                    i_startPos = i_endPos;
                    i_endPos =  s_input.find(",", i_startPos+1);
                    if (i_endPos == string::npos || i_endPos > i_endBracketPos)
                        i_endPos = i_endBracketPos;
                    s_origArg.assign(s_input, i_startPos+1, i_endPos - i_startPos-1);
            //cout << " s_origArg: " << s_origArg << endl;
                    v_origArg.push_back(s_origArg);
                }

                map_orig_arg.insert( pair<string,vector<string> >(s_origFunc2, v_origArg) );
                map_rand_orig_func.insert( pair<string,string>(s_origFunc2, s_randFunc) );


            }

            else if (i_quotePos > i_numSpaces && i_quotePos < i_singleQuotePos && i_quotePos != s_input.size()) {
                i_startPos = s_input.find(" ", i_startPos);
                i_endPos =  s_input.find(" ", i_startPos+1);
                s_randName.assign(s_input, i_startPos+1, i_endPos - i_startPos-1);
            cout << " s_randName: " << s_randName << endl;
                i_startPos = s_input.find("\"", i_endPos+1);
                i_endPos = i_startPos;
                skipQuote(s_input, i_endPos, '\"');
               // i_endPos =  s_input.find("\"", i_startPos+1);
                s_origName.assign(s_input, i_startPos, i_endPos - i_startPos+1);
            cout << " s_origName: " << s_origName << endl;
                m_output.insert(pair<string,string>(s_randName, s_origName));
            }

            else if (i_singleQuotePos > i_numSpaces && i_singleQuotePos != s_input.size()) {
                i_startPos = s_input.find(" ", i_startPos);
                i_endPos =  s_input.find(" ", i_startPos+1);
                s_randName.assign(s_input, i_startPos+1, i_endPos - i_startPos-1);
                i_startPos = s_input.find("\'", i_endPos+1);
                i_endPos = i_startPos;
                skipQuote(s_input, i_endPos, '\'');
                s_origName.assign(s_input, i_startPos, i_endPos - i_startPos+1);
                m_output.insert(pair<string,string>(s_randName, s_origName));
            }
    }

    map<string, vector<string> >::iterator m_mIter;
    map<string, vector<string> >::iterator m_mIter2;
    string s_tempOrigiFunc;
    vector<string> v_tempRandArg, v_tempOrigArg;

    string s_originalName, s_tempRandFunc;
    vector<string> v_tempPosArg;
    map<string,string> m_temp_rand_orig_arg;

    for(m_mIter=map_rand_arg.begin(); m_mIter != map_rand_arg.end();
            ++m_mIter) {
        s_tempOrigiFunc = (*m_mIter).first;
        v_tempRandArg = (*m_mIter).second;
        v_tempOrigArg = map_orig_arg[ s_tempOrigiFunc ];

        map_rand_orig_arg.clear();
        for (int i=0; i<v_tempRandArg.size(); ++i) {
            if (i == v_tempOrigArg.size())
                break;

            map_rand_orig_arg.insert( pair<string, string>(v_tempRandArg[i],v_tempOrigArg[i]));

        }

        s_originalName.clear();
        v_tempPosArg = map_pos_arg[ s_tempOrigiFunc ];
        for (int i=0; i<v_tempPosArg.size(); ++i) {
            s_originalName += map_rand_orig_arg[ v_tempPosArg[i] ];
            cout << " v_tempPosArg[i] : " << v_tempPosArg[i] << endl;
            cout << " s_originalName : " << s_originalName << endl;

        }
        s_tempRandFunc = map_rand_orig_func[ s_tempOrigiFunc ];
        m_output.insert( pair<string,string>(s_tempRandFunc, s_originalName) );
    }


}


void AlgorithmDefine::listOfInclude(string& s_input, vector<string>& v_output)
{
    string s_alpha = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    int i_pos = 0, i_end, i_quotePos;
    string s_randName;
    vector<string> v_listOfRandName;
    while (s_input.find("include", i_pos+1) != string::npos) {
        i_pos = s_input.find("include", i_pos+1);
        i_quotePos =  s_input.find("\"", i_pos+1);
        i_pos = s_input.find(" ", i_pos+1);
        i_pos = s_input.find_first_of(s_alpha, i_pos+1);

        if (i_quotePos < i_pos && i_quotePos != string::npos) {
            i_pos = i_quotePos;
            i_end = i_quotePos;
            skipQuote(s_input, i_end, '\"');
        }

        else {

                i_end = s_input.find_first_not_of(s_alpha, i_pos+1);
        }
        s_randName.assign(s_input, i_pos, i_end-i_pos);
        v_listOfRandName.push_back(s_randName);
    }
    v_output = v_listOfRandName;
    // todo : get from map the original names
}

void AlgorithmDefine::getMappingReverseRandom(string& s_input, map<string,string>& m_output)
{
        if(s_input.empty())
            return;

        string s_string;
        size_t i_firstDelim = 0, i_secondDelim = 0;
        vector<string> v_string;
	m_output.clear();

        i_firstDelim = s_input.find_first_of(';');
        i_secondDelim = s_input.find_first_of(';',i_firstDelim+1);

        if (i_secondDelim > s_input.find_first_of("\r\n"))
            return;

        s_string.assign(s_input.begin(), s_input.begin()+i_firstDelim);
        v_string.push_back(s_string);

        while(i_secondDelim != string::npos) {
            s_string.assign(s_input.begin()+i_firstDelim+1, s_input.begin()+i_secondDelim);
            v_string.push_back(s_string);

            i_firstDelim = s_input.find_first_of(';',i_firstDelim+1);
            i_secondDelim = s_input.find_first_of(';',i_secondDelim+1);
        }

        i_firstDelim =  0;
        i_secondDelim =  0;

	for(int i=0; i<v_string.size(); ++i)
	{
		m_output.insert (pair<string,string>(v_string[i],v_string[i+1]) );
		++i;
	}
}

void AlgorithmDefine::formatWords(string& str_output)
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


void AlgorithmDefine::removeSpace( string& str_output )
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

void AlgorithmDefine::removeNewline( string& s_input, vector<string>& v_output)
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
