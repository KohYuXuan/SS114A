
#include "AlgorithmDefine.h"

bool AlgorithmDefine::algorithm(string& s_input, string& s_sourceOutput, string& s_headerOutput,
                                vector<string>& v_listOfHeaderFiles, bool& b_addHeaderFile)
{
    m_listOfDefine.clear();
    m_listOfInclude.clear();
    m_listOfBlocks.clear();

    int i_startQuote=0, i_endQuote=0, i_pos=0, i_num;

    string s_chars, s_rand, s_output,
            s_headerFile, s_randName,s_block
            , s_randBracketOpen, s_randBracketClose, s_randComma;

    s_randName = RAND_NAME;
    i_num = NUM_NAME;

    while (i_pos < s_input.size()) {
        s_output.clear();

        if (s_input[i_pos] == '\"'){
            i_startQuote = i_pos;
            skipQuote(s_input, i_pos, '\"');
            i_endQuote = i_pos;
            s_output.assign(s_input, i_startQuote, i_endQuote-i_startQuote);
            s_rand = nameGenerator(s_randName, i_num);
            s_block =  s_rand + " ";
            m_listOfBlocks.push_back(s_block);
            s_headerFile = "\n#define " + s_rand + " " + s_output;
            m_listOfDefine.push_back(s_headerFile);
            continue;
        }

        if (s_input[i_pos] == '\'') {
            i_startQuote = i_pos;
            skipQuote(s_input, i_pos, '\'');
            i_endQuote = i_pos;
            s_output.assign(s_input, i_startQuote, i_endQuote-i_startQuote);
            s_rand = nameGenerator(s_randName, i_num);
            s_block =  s_rand + " ";
            m_listOfBlocks.push_back(s_block);
            s_headerFile = "\n#define " + s_rand + " " + s_output;
            m_listOfDefine.push_back(s_headerFile);
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

        if(s_input[i_pos] == '#' && s_input[i_pos+1] == 'i' &&
            s_input[i_pos+2] == 'n' && s_input[i_pos+3] == 'c' ){ // check for #include
                int i_tempPos = i_pos;
                skipHeader(s_input, i_pos, '>');

                int i_bracketPos = i_tempPos;
                i_bracketPos  = s_input.find("<", i_bracketPos); // if "<" is after ">", means double quote is used
                if (i_bracketPos > i_pos || i_bracketPos == string::npos) {
                    s_output.assign(s_input, i_tempPos, i_pos-i_tempPos);

                    int i_startQuotePos = 0, i_endQuotePos = 0;
                    string s_headerName;
                    b_addHeaderFile = true;
                    i_startQuotePos = s_input.find("\"", i_tempPos);
                    i_endQuotePos = s_input.find("\"", i_startQuotePos+1);
                    s_headerName.assign(s_input, i_startQuotePos+1, i_endQuotePos-i_startQuotePos-1);
                    for (int i=0; i<v_listOfHeaderFiles.size(); ++i) {
                        if (v_listOfHeaderFiles[i].find(s_headerName) != string::npos) {
                            s_block = "\n" + s_output + "\n";
                            m_listOfBlocks.push_back(s_block);
                            b_addHeaderFile = false;
                        }
                    }

                    if(b_addHeaderFile == false)
                        continue;

                    s_output.insert(0,"\n");
                    m_listOfInclude.push_back(s_output);
                    continue;
                }

                int i_exceptionWord = i_tempPos;
                i_exceptionWord  = s_input.find("errno", i_bracketPos); // errno.h cannot be obfuscated
                if (i_exceptionWord < i_pos && i_exceptionWord != string::npos) {
                    s_output.assign(s_input, i_tempPos, i_pos-i_tempPos);
                    s_output.insert(0,"\n");
                    m_listOfInclude.push_back(s_output);
                    continue;
                }

                s_chars.assign(s_input, i_bracketPos, i_pos-i_bracketPos);
                modifyString(s_chars,s_rand,s_headerFile);
                m_listOfDefine.push_back(s_headerFile);
                s_output = "\n#include " + s_rand;
                m_listOfInclude.push_back(s_output);
                s_chars.clear();
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

        if(s_input[i_pos] == '(' ){
                ++i_pos;
                s_output = " ( ";
                s_rand = nameGenerator(s_randName, i_num);

                if (!s_randBracketOpen.empty()) {
                    m_listOfBlocks.push_back(s_randBracketOpen);
                    continue;
                }

                s_randBracketOpen = s_rand + " ";
                m_listOfBlocks.push_back(s_randBracketOpen);
                s_headerFile = "\n#define " + s_rand + " " + s_output;
                m_listOfDefine.push_back(s_headerFile);
                continue;
        }

        if(s_input[i_pos] == ')' ){
                ++i_pos;
                s_output = " ) ";
                s_rand = nameGenerator(s_randName, i_num);

                if (!s_randBracketClose.empty()) {
                    m_listOfBlocks.push_back(s_randBracketClose);
                    continue;
                }
                s_randBracketClose = s_rand + " ";
                m_listOfBlocks.push_back(s_randBracketClose);
                s_headerFile = "\n#define " + s_rand + " " + s_output ;
                m_listOfDefine.push_back(s_headerFile);
                continue;
        }

        if(s_input[i_pos] == ',' ){
                ++i_pos;
                s_output = " , ";
                s_rand = nameGenerator(s_randName, i_num);

                if (!s_randComma.empty()) {
                    m_listOfBlocks.push_back(s_randComma);
                    continue;
                }
                s_randComma = s_rand + " ";
                m_listOfBlocks.push_back(s_randComma);
                s_headerFile = "\n#define " + s_rand + " " + s_output;
                m_listOfDefine.push_back(s_headerFile);
                continue;
        }

        if(s_input[i_pos] == '/' && s_input[i_pos+1] == '/'){ // check for #include
                removeComment(s_input, i_pos);
                continue;
        }

        if(s_input[i_pos] == '/' && s_input[i_pos+1] == '*'){ // check for #include
                removeComment(s_input, i_pos);
                continue;
        }

        s_output.clear();
        s_chars.clear();

        while (!isspace(s_input[i_pos]) && i_pos < s_input.size()) {
            if (s_input[i_pos] == '(') {
                --i_pos;
                break;
            }
            if (s_input[i_pos] == ')') {
                --i_pos;
                break;
            }

            if (s_input[i_pos] == '\"') {
                --i_pos;
                break;
            }
            if (s_input[i_pos] == '\'') {
                --i_pos;
                break;
            }

            if (s_input[i_pos] == ',') {
                --i_pos;
                break;
            }

            if (s_input[i_pos] == '/' && s_input[i_pos+1] == '/') {
                --i_pos;
                break;
            }

            if (s_input[i_pos] == '/' && s_input[i_pos+1] == '*') {
                --i_pos;
                break;
            }

            s_chars.push_back(s_input[i_pos]);
            ++i_pos;
        }

        modifyString(s_chars,s_rand,s_headerFile);

        if (!s_chars.empty())
            s_output += s_rand + " ";

        if (!s_output.empty()) {
            s_block =  s_rand + " ";
            m_listOfBlocks.push_back(s_block);

        }

        ++i_pos;
    }

	string s_formated;
	format(m_listOfBlocks,s_formated);
	s_sourceOutput = s_formated;

	int i_intRand;
	set<int> set_checkDuplicate;

	while (set_checkDuplicate.size() != m_listOfDefine.size()) {
		i_intRand = rand() % m_listOfDefine.size();
		if(set_checkDuplicate.insert(i_intRand).second == true)
			s_headerOutput += m_listOfDefine[i_intRand];
	}
	for (int i=0; i<m_listOfInclude.size(); ++i)
		s_headerOutput += m_listOfInclude[i];
}

void AlgorithmDefine::modifyString(string& s_input, string& s_randOutput, string& s_output)
{
	int i_num;
	string s_name, s_name2, s_argument, s_randName, s_headerFile;
	vector<int> v_charPos;
	vector<string> v_listOfArgument;

	if (s_input.empty())
		return;

    if (s_input.size() > MAX_CHARACTER_ALLOWED / 2) {

        string s_rand, s_block;
        i_num = NUM_NAME;
        s_randName = RAND_NAME;
        s_rand = nameGenerator(s_randName, i_num);

        s_block =  s_rand + " ";
        m_listOfBlocks.push_back(s_block);

        s_headerFile = "\n#define " + s_rand + " " + s_input;
        m_listOfDefine.push_back(s_headerFile);

        s_input.clear();
        s_randOutput.clear();

        return;
    }

	randomiseString(s_input, v_charPos);

	i_num = NUM_NAME;
	s_randName = RAND_NAME;

	for (int i=0; i<s_input.size(); ++i) {
		s_argument = nameGenerator(s_randName, i_num);
		v_listOfArgument.push_back(s_argument);
	}

	vector<string> v_argumentArrange;
	for (int i=0; i<v_charPos.size(); ++i) {
		if (v_charPos[i] != -1)
			v_argumentArrange.push_back( v_listOfArgument[v_charPos[i]] );
	}

	i_num = NUM_NAME;
	s_randName = RAND_NAME;
	s_name = nameGenerator(s_randName, i_num);
	s_name2 = nameGenerator(s_randName, i_num);

	s_headerFile = "\n#define " + s_name + "(";
	for (int i=0; i<v_listOfArgument.size(); ++i) {
		s_headerFile += v_listOfArgument[i];
		if (i != v_listOfArgument.size()-1)
			s_headerFile.push_back(',');

	}
	s_headerFile += ")";
	for (int i=0; i<v_argumentArrange.size(); ++i) {
		s_headerFile += v_argumentArrange[i];
		if (i == v_argumentArrange.size()-1)
			break;

		if (i != v_argumentArrange.size()-1
			&& isalnum(s_input[v_charPos[i]]) && isalnum(s_input[v_charPos[i+1]])
			|| s_input[v_charPos[i]] == '_' || s_input[v_charPos[i+1]] == '_'
			|| s_input[v_charPos[i]] == '<' && s_input[v_charPos[i+1]] == '<'
			|| s_input[v_charPos[i]] == '>' && s_input[v_charPos[i+1]] == '>'
			|| s_input[v_charPos[i]] == '<' && s_input[v_charPos[i+1]] == '='
			|| s_input[v_charPos[i]] == '>' && s_input[v_charPos[i+1]] == '='
			|| s_input[v_charPos[i]] == '=' && s_input[v_charPos[i+1]] == '='
			|| s_input[v_charPos[i]] == '!' && s_input[v_charPos[i+1]] == '='
			|| s_input[v_charPos[i]] == '%' && s_input[v_charPos[i+1]] == '='
			|| s_input[v_charPos[i]] == '^' && s_input[v_charPos[i+1]] == '='
			|| s_input[v_charPos[i]] == '*' && s_input[v_charPos[i+1]] == '='
			|| s_input[v_charPos[i]] == '+' && s_input[v_charPos[i+1]] == '='
			|| s_input[v_charPos[i]] == '+' && s_input[v_charPos[i+1]] == '+'
			|| s_input[v_charPos[i]] == '-' && s_input[v_charPos[i+1]] == '-'
			|| s_input[v_charPos[i]] == '-' && s_input[v_charPos[i+1]] == '='
			|| s_input[v_charPos[i]] == '/' && s_input[v_charPos[i+1]] == '='
            || s_input[v_charPos[i]] == '|' && s_input[v_charPos[i+1]] == '|'
			|| s_input[v_charPos[i]] == ':' && s_input[v_charPos[i+1]] == ':'
			|| s_input[v_charPos[i]] == '-' && s_input[v_charPos[i+1]] == '>'
			|| s_input[v_charPos[i]] == '&' && s_input[v_charPos[i+1]] == '&'
			|| s_input[v_charPos[i]] == '.' && isdigit(s_input[v_charPos[i+1]]) && isdigit(s_input[v_charPos[i-1]])
			|| isdigit(s_input[v_charPos[i]]) && s_input[v_charPos[i+1]] == '.' && isdigit(s_input[v_charPos[i+2]]) )
			s_headerFile += "##";
		else
			s_headerFile += " ";
	}

	s_output += s_headerFile;
	m_listOfDefine.push_back(s_headerFile);

	s_headerFile = "\n#define " + s_name2 + " " + s_name + "(";
	for (int i=0; i<s_input.size(); ++i) {
		s_headerFile.push_back(s_input[i]);
		if (i != s_input.size()-1)
			s_headerFile.push_back(',');

	}

	s_headerFile += ")";
	s_output += s_headerFile;
	m_listOfDefine.push_back(s_headerFile);
	s_randOutput = s_name2;
}

void AlgorithmDefine::writeToFile(const string& s_fileName,const string& s_input)
{
       ofstream out(s_fileName.c_str());
       out << s_input;
       out.close();
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

void AlgorithmDefine::skipHeader(const string& s_input, int& i_skip, char c_bracket)
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

void AlgorithmDefine::randomiseString(string& s_inOutput, vector<int>& v_charPos)
{
	int i_randNum=0, i_randNum1=0, i_pos=0;
	string s_output, s_checkForDuplicate, s_redundancy;
	string s_rand = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_=;-+!:<>{}[]@$%^&*";
	set<int> set_randNum;
	set<int> set_randNum1;

	v_charPos.clear();
	while (set_randNum.size() != (s_inOutput.size() * ADD_REDUNDANCY) ) {
		i_randNum = rand() % MAX_CHARACTER_ALLOWED;
		if (set_randNum.insert(i_randNum).second == true) {
			while (s_inOutput.find(s_rand[i_randNum]) != string::npos) {
				i_randNum = rand() % MAX_CHARACTER_ALLOWED;
			}
			s_redundancy.push_back(s_rand[i_randNum]);
		}
	}

	set_randNum.clear();
	int i=0;
	vector<int> v_tempCharPos;
	while (set_randNum.size() != s_inOutput.size() ) {
		i_randNum = rand() % s_inOutput.size();
		if (set_randNum.insert(i_randNum).second == true) {
			i_randNum1 = rand() % s_redundancy.size();

			while (set_randNum1.insert(i_randNum1).second != true) {
				i_randNum1 = rand() % s_redundancy.size();
			}

            i_pos = s_checkForDuplicate.find(s_inOutput[i]);

            if (i_pos == string::npos) {
                s_checkForDuplicate.push_back(s_inOutput[i]);
                s_redundancy.replace(i_randNum1, 1, 1, s_inOutput[i]);
                ++i;
                v_tempCharPos.push_back(i_randNum1);
			}
            else {
                i_pos = s_redundancy.find(s_inOutput[i]);
                v_tempCharPos.push_back(i_pos);
                ++i;
            }
		}
	}
    v_charPos = v_tempCharPos;
    s_inOutput = s_redundancy;

}

string AlgorithmDefine::nameGenerator(string& s_name, int i_num)
{
	string s_output;
	int i_randNum=0;

    for (int i=0; i < i_num; ++i) {
        i_randNum = rand() % s_name.size();
        s_output.push_back(s_name[i_randNum]);
    }

    while (m_duplicateCheck.insert(s_output).second == false) {
        s_output.clear();
        for (int i=0; i < i_num; ++i) {
            i_randNum = rand() % s_name.size();
            s_output.push_back(s_name[i_randNum]);
        }
    }
	return s_output;
}

void AlgorithmDefine::format(vector<string>& v_input, string& s_output)
{
    int i_line = 0;
    int i_numBlocks = v_input.size();

    if (i_numBlocks <= 48 + 7)  {  //48 + 7)

            for (int j=0; j<v_input.size(); ++j) {
                s_output += v_input[j] + " ";
                ++i_line;
                if ((i_line % BLOCKS_PER_ROW) == 0) {
                    i_line = 0;
                    s_output += "\n";
                }
        }
        return;
    }
    i_numBlocks -= 48;

    float i_quotients = 0.0;
    i_quotients = i_numBlocks / BLOCKS_PER_ROW;
    int i_linesOfBlocksOnTop = 0;

    if (i_quotients > 1) {
        i_linesOfBlocksOnTop = i_quotients / 2;
        i_line = 1;

        for (int k=0; k<i_linesOfBlocksOnTop*BLOCKS_PER_ROW; ++k) {
                s_output += v_input[k] + " ";

                if ((i_line % BLOCKS_PER_ROW) == 0) {
                    s_output += "\n";
                }
                ++i_line;
        }
    }

    int i_patternPos = 0;
    i_patternPos = i_linesOfBlocksOnTop * BLOCKS_PER_ROW;

    s_output +=
        // first line
        addSpace(5)
        + v_input[i_patternPos] + addSpace(21)
        + v_input[i_patternPos+1] + " \n"

        // second line
        + addSpace(4)
        + v_input[i_patternPos+2] + addSpace(3)
        + v_input[i_patternPos+3] + addSpace(7)
        + v_input[i_patternPos+4] + addSpace(3)
        + v_input[i_patternPos+5] + " \n"

        // third line
        + addSpace(3)
        + v_input[i_patternPos+6] + addSpace(7)
        + v_input[i_patternPos+7] + addSpace(1)
        + v_input[i_patternPos+8] + addSpace(7)
        + v_input[i_patternPos+9] + " \n"

        // fourth line
        + addSpace(2)
        + v_input[i_patternPos+10] + addSpace(11)
        + v_input[i_patternPos+11] + addSpace(11)
        + v_input[i_patternPos+12] + " \n"

        // fifth line
        + addSpace(1)
        + v_input[i_patternPos+13] + addSpace(9)
        + v_input[i_patternPos+14] + addSpace(1)
        + v_input[i_patternPos+15] + addSpace(9)
        + v_input[i_patternPos+16] + " \n"

        // sixth line
        + addSpace(2)
        + v_input[i_patternPos+17] + addSpace(5)
        + v_input[i_patternPos+18] + addSpace(7)
        + v_input[i_patternPos+19] + addSpace(5)
        + v_input[i_patternPos+20] + " \n"

        // seven line
        + addSpace(3)
        + v_input[i_patternPos+21] + addSpace(25)
        + v_input[i_patternPos+22] + " \n"

        // letter 'E', 25 blocks
        // first line
        + addSpace(4)
        + v_input[i_patternPos+23] + addSpace(23)
        + v_input[i_patternPos+24] + " \n"

        // second line
        + addSpace(3)
        + v_input[i_patternPos+25] + addSpace(4)
        + v_input[i_patternPos+26] + addSpace(1)
        + v_input[i_patternPos+27] + addSpace(1)
        + v_input[i_patternPos+28] + addSpace(4)
        + v_input[i_patternPos+29] + " \n"

        // third line
        + addSpace(2)
        + v_input[i_patternPos+30] + addSpace(5)
        + v_input[i_patternPos+31] + addSpace(17)
        + v_input[i_patternPos+32] + " \n"

        // fourth line
        + addSpace(1)
        + v_input[i_patternPos+33] + addSpace(6)
        + v_input[i_patternPos+34] + addSpace(1)
        + v_input[i_patternPos+35] + addSpace(1)
        + v_input[i_patternPos+36] + addSpace(6)
        + v_input[i_patternPos+37] + " \n"

        // fifth line
        + addSpace(2)
        + v_input[i_patternPos+38] + addSpace(5)
        + v_input[i_patternPos+39] + addSpace(17)
        + v_input[i_patternPos+40] + " \n"

        // sixth line
        + addSpace(3)
        + v_input[i_patternPos+41] + addSpace(4)
        + v_input[i_patternPos+42] + addSpace(1)
        + v_input[i_patternPos+43] + addSpace(1)
        + v_input[i_patternPos+44] + addSpace(4)
        + v_input[i_patternPos+45] + " \n"

        // seven line
        + addSpace(4)
        + v_input[i_patternPos+46] + addSpace(23)
        + v_input[i_patternPos+47] + " \n";

        i_patternPos += 48;
        i_line = 0;
        for (int k=i_patternPos; k<v_input.size(); ++k) {
                s_output += v_input[k] + " ";
                ++i_line;
                if ((i_line % BLOCKS_PER_ROW) == 0) {
                    i_line = 0;
                    s_output += "\n";
                }
        }
}

string AlgorithmDefine::addSpace(int i_numSpaces)
{
	string s_output;
	for (int i=0; i< i_numSpaces; ++i)
		s_output += " ";

	return s_output;
}

char rand_alnum()
{
    char c;
    while (!std::isalpha(c = static_cast<char>(std::rand())));
    return c;
}

std::string AlgorithmDefine::rand_alnum_str (std::string::size_type sz)
{
    std::string s;
    s.reserve  (sz);
    generate_n (std::back_inserter(s), sz, rand_alnum);
    return s;
}

void AlgorithmDefine::removeComment(string& s_input, int& i_skip)
{
	int i = i_skip, i_toRemove = 0;

	if(s_input[i] == '/' && s_input[i+1] == '/')
	{
		i_toRemove = s_input.find("\n",i);
		s_input.erase(i, i_toRemove-i);
	}
	else if(s_input[i] == '/' && s_input[i+1] == '*')
	{
		i_toRemove = s_input.find("*/",i);
		s_input.erase(i, i_toRemove-i+2);

	}
}
