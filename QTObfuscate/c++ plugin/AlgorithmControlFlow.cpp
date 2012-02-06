
#include "AlgorithmControlFlow.h"

void AlgorithmControlFlow::getOutputBuffer(string& s_output)
{
	s_output = m_saveToBuffer;
}

void AlgorithmControlFlow::writeToBuffer(const string& s_input)
{
	m_saveToBuffer += s_input;
}

char rand_alnum()
{
    char c;
    while (!std::isalpha(c = static_cast<char>(std::rand())));
    return c;
}

std::string AlgorithmControlFlow::rand_alnum_str (std::string::size_type sz)
{
    std::string s;
    s.reserve  (sz);
    generate_n (std::back_inserter(s), sz, rand_alnum);
    return s;
}

string AlgorithmControlFlow::intToString(int i_input)
{
	stringstream out;
	out << i_input;
	return out.str();
}

void AlgorithmControlFlow::control_flow_flattening(string& s_input, set<string>& set_dataTypes)
{
	int i_startPosition = 0, i_endPosition = 0, i_tempStartPosition;
	string s_output, s_tempOutput, s_functionBody, s_addBracket="}";

	findFunction(s_input, i_startPosition, i_endPosition);
	s_output.assign(s_input,0,i_startPosition+1);
	writeToBuffer(s_output);
	s_output.clear();

	while (i_startPosition != string::npos) {
		s_functionBody.assign(s_input, i_startPosition+1, i_endPosition-i_startPosition-1);
		vector<string> v_listOfType(set_dataTypes.begin(), set_dataTypes.end());

		for (int i=0; i<v_listOfType.size(); ++i)
		{ 	while (moveVariable(s_functionBody, v_listOfType[i], s_tempOutput)) {
				s_output += s_tempOutput + "\n";
				s_tempOutput.clear();
			}
		}
        writeToBuffer(s_output);
		flatten_block(s_functionBody);
		s_output.clear();
		writeToBuffer(s_addBracket);
		s_functionBody.clear();

		i_tempStartPosition = i_endPosition;
		findFunction(s_input, i_startPosition, i_endPosition);

		if (i_startPosition != string::npos) {
			s_output.assign(s_input, i_tempStartPosition+1, i_startPosition-i_tempStartPosition);
			writeToBuffer(s_output);
			s_output.clear();
		}
	}
}

void AlgorithmControlFlow::flatten_block(string& s_block)
{
	string s_whileLabel = rand_alnum_str(NUM_OF_RANDOM_CHAR);
	string s_switchVariable = rand_alnum_str(NUM_OF_RANDOM_CHAR);
	int i_entry = RANDOM_NUM;
	int i_exit = RANDOM_NUM;

	string s_entry, s_exit;

	s_entry = intToString(i_entry);
	s_exit = intToString(i_exit);

	s_output = "int " + s_switchVariable + " = " + s_entry + ";" + "\n"
		+ s_whileLabel + ":" + "\n"
		+ "while (" + s_switchVariable + " != " + s_exit + ") {" + "\n"
		+ " switch (" + s_switchVariable + ") {" + "\n";

	writeToBuffer(s_output);

	m_levels.push_back(pair<string,string>(s_switchVariable,s_whileLabel));
	transform_block(s_block, i_entry, i_exit);
	m_levels.pop_back();

	s_output.clear();
	s_output += "	}\n}\n";

	writeToBuffer(s_output);
}

void AlgorithmControlFlow::transform_block(string &s_input, int &i_entry, int i_exit)
{
    vector<pair<string,string> > m_blockParts;
	string s_sequence;
	string s_compound;
	string s_earliest;

	int i_earliest = 0,
		i_sequence = 0,
		i_if = 0,
		i_while = 0,
		i_do = 0,
		i_for = 0,
		i_try = 0,
		i_switch = 0;
	vector<int> v_listNumElseIf;
	vector<bool> v_listElseExist;

	while (true) {
		i_if = s_input.find("if",i_earliest);
		i_while = s_input.find("while",i_earliest);
		i_do = s_input.find("do",i_earliest);
		i_for = s_input.find("for",i_earliest);
		i_try = s_input.find("try",i_earliest);
		i_switch = s_input.find("switch",i_earliest);

		i_earliest = s_input.size();
		s_earliest = "";

		if (i_if == string::npos &&
			i_while == string::npos &&
			i_do == string::npos &&
			i_for == string::npos &&
			i_try == string::npos &&
			i_switch == string::npos)
		{
			// add sequence of statement
			if (i_sequence != string::npos)
				s_sequence.assign(s_input,i_sequence,s_input.size());

			if (!s_sequence.empty())
				m_blockParts.push_back(pair<string,string>("sequence",s_sequence));

			break;
		}

		if (i_if != string::npos){
			// make sure the "if" match the whole word and not inbetween words
			// and also will not match "else if"
			while (s_input[i_if-5] == 'e' && s_input[i_if-4] == 'l' &&
				s_input[i_if-3] == 's' && s_input[i_if-2] == 'e' ||
				isalnum(s_input[i_if-1]) || isalnum(s_input[i_if+2])
				&& i_if != string::npos)
			{
				i_if = s_input.find("if", i_if+1);
			}

			if (i_earliest > i_if && i_if != string::npos){
				i_earliest = i_if;
				s_earliest = "if";
			}

			if (i_if == string::npos &&
				i_while == string::npos &&
				i_do == string::npos &&
				i_for == string::npos &&
				i_try == string::npos &&
				i_switch == string::npos)
			{
				// add sequence of statement
				if (i_sequence != string::npos)
					s_sequence.assign(s_input,i_sequence,s_input.size());

				if (!s_sequence.empty())
					m_blockParts.push_back(pair<string,string>("sequence",s_sequence));

				break;
			}
		}

		if (i_while != string::npos){
			// make sure the "while" match the whole word and not inbetween words
			if (!isalnum(s_input[i_while-1]) && !isalnum(s_input[i_while+5])) {
				if (i_earliest > i_while){
					i_earliest = i_while;
					s_earliest = "while";
				}
			}
		}

		if (i_do != string::npos){
			// make sure the "do" match the whole word and not inbetween words
			if (!isalnum(s_input[i_do-1]) && !isalnum(s_input[i_do+2])) {
				if (i_earliest > i_do){
					i_earliest = i_do;
					s_earliest = "do";
				}
			}
		}

		if (i_for != string::npos){
			// make sure the "for" match the whole word and not inbetween words
			if (!isalnum(s_input[i_for-1]) && !isalnum(s_input[i_for+3])) {
				if (i_earliest > i_for){
					i_earliest = i_for;
					s_earliest = "for";
				}
			}
		}

		if (i_try != string::npos){
			// make sure the "try" match the whole word and not inbetween words
			if (isspace(s_input[i_try-1]) && isspace(s_input[i_try+3])
                || isspace(s_input[i_try-1]) && s_input[i_try+3] == '{'   ) {
				if (i_earliest > i_try){
					i_earliest = i_try;
					s_earliest = "try";
				}
			}
		}

		if (i_switch != string::npos){
			// make sure the "try" match the whole word and not inbetween words
			if (!isalnum(s_input[i_switch-1]) && !isalnum(s_input[i_switch+6])) {
				if (i_earliest > i_switch){
					i_earliest = i_switch;
					s_earliest = "switch";
				}
			}
		}

		int i_posOfEndPosition=0;
		searchEndPosition(s_input, i_earliest, i_posOfEndPosition);

		int i_numElseIf = 0;
		bool b_elseExist = false;
		if (s_earliest == "if") {
			searchEndOfElseIf(s_input, i_posOfEndPosition, i_posOfEndPosition, i_numElseIf, b_elseExist);
			v_listNumElseIf.push_back(i_numElseIf);
			v_listElseExist.push_back(b_elseExist);
		}
		else if (s_earliest == "do")
			i_posOfEndPosition = s_input.find(";", i_posOfEndPosition);
		else if (s_earliest == "for") {
			int i_tempEndPos;
			searchEndBraces(s_input, i_earliest, i_tempEndPos);
			searchEndPosition(s_input, i_tempEndPos, i_posOfEndPosition);
		}
		else if (s_earliest == "try") {
			int i_posNextTry = 0;
			s_input.find("try", i_posOfEndPosition);
			while (i_posOfEndPosition > i_posNextTry && i_posNextTry != string::npos) {
				while (!isalnum(s_input[i_posNextTry-1]) && !isalnum(s_input[i_posNextTry+3])
					&& i_posNextTry != string::npos)
				{
					i_posNextTry = s_input.find("try", i_posNextTry+1);
				}
				if (i_posNextTry == string::npos)
					i_posNextTry = s_input.size();

				int i_tempEndPos = 0;
				i_tempEndPos = s_input.rfind("catch", i_posNextTry);
				searchEndPosition(s_input, i_tempEndPos, i_posOfEndPosition);
			}
		}

		// add sequence of statement
		s_sequence.assign(s_input,i_sequence,i_earliest-i_sequence);

		if (!s_sequence.empty())
			m_blockParts.push_back(pair<string,string>("sequence",s_sequence));

		// add compound statement
		s_compound.assign(s_input,i_earliest,i_posOfEndPosition-i_earliest+1);

		i_earliest = i_posOfEndPosition;
		if (!s_compound.empty())
			m_blockParts.push_back(pair<string,string>(s_earliest,s_compound));

		i_sequence = i_earliest+1;
	}

	int i_partExit = 0, i_temp = 0;
        for(int i=0; i<m_blockParts.size(); ++i)
        {
		i_partExit = (i == m_blockParts.size()-1) ? i_exit : RANDOM_NUM;
		if (m_blockParts[i].first.compare("if") == 0) {
			if (i_temp >= v_listNumElseIf.size() || i_temp >= v_listElseExist.size())
				cout << "vector out of bound!\n";

			transform_if(m_blockParts[i].second,i_entry,i_partExit,v_listNumElseIf[i_temp],v_listElseExist[i_temp]);
			++i_temp;
		}

		if (m_blockParts[i].first.compare("while") == 0) {
			transform_while(m_blockParts[i].second,i_entry,i_partExit);
		}

		if (m_blockParts[i].first.compare("for") == 0) {
			transform_for(m_blockParts[i].second,i_entry,i_partExit);
		}

		if (m_blockParts[i].first.compare("do") == 0) {
			transform_do(m_blockParts[i].second,i_entry,i_partExit);
		}

		if (m_blockParts[i].first.compare("try") == 0) {
			transform_try(m_blockParts[i].second,i_entry,i_partExit);
		}

		if (m_blockParts[i].first.compare("sequence") == 0) {
			transform_sequence(m_blockParts[i].second,i_entry,i_partExit);
		}

		if (m_blockParts[i].first.compare("switch") == 0) {
			transform_switch(m_blockParts[i].second,i_entry,i_partExit);
		}

		i_entry = i_partExit;
	}
}

void AlgorithmControlFlow::transform_if(string &s_input, int i_entry, int i_exit, int i_numElseIf, bool b_elseExist)
{
	string s_switchVariable = m_levels.back().first;
	int i_thenEntry = RANDOM_NUM;
	int i_elseEntry = b_elseExist ? RANDOM_NUM : i_exit;

	string s_entry, s_exit, s_thenEntry, s_elseEntry;

	s_thenEntry = intToString(i_thenEntry);
	s_elseEntry = intToString(i_elseEntry);
	s_entry = intToString(i_entry);
	s_exit = intToString(i_exit);

	// "if" predicate
	int i_startPredicate = 0, i_endPredicate = 0;
	string s_ifPredicate;
	i_startPredicate = s_input.find("(");
	searchEndBraces(s_input, i_startPredicate, i_endPredicate);
	s_ifPredicate.assign(s_input,i_startPredicate+1,i_endPredicate-i_startPredicate-1);

	 // "if" statement
	int i_startBody = 0, i_endBody = 0;
	string s_ifBody;
	i_startBody = s_input.find(")");
	searchEndPosition(s_input, i_startBody, i_endBody);

	int i_checkBracket=0, i_checkSemiColon=0;
	i_checkBracket = s_input.find("{",i_startBody);
	i_checkSemiColon = s_input.find(";",i_startBody);
	if (i_checkSemiColon > i_checkBracket && i_checkBracket != string::npos) {
		i_startBody = i_checkBracket;
		s_ifBody.assign(s_input,i_startBody+1,i_endBody-i_startBody-1);
	}
	else
		s_ifBody.assign(s_input,i_startBody+1,i_endBody-i_startBody);

	 // "else if" predicate and statement
	vector<string> v_list_ElseIfBody, v_list_ElseIfPredicate;
	vector<int> v_list_elseIfEntry;
	string s_elseIfBody, s_elseIfPredicate;
	//i_startBody = 0;

	while (i_numElseIf != 0) {
		i_startBody = i_endBody;
		--i_numElseIf;
		v_list_elseIfEntry.push_back(RANDOM_NUM);
		i_startBody = s_input.find("else if", i_startBody);
		i_startPredicate = s_input.find("(", i_startBody);
		i_endPredicate = s_input.find(")",i_startPredicate);
		s_elseIfPredicate.assign(s_input,i_startPredicate+1,i_endPredicate-i_startPredicate-1);
		v_list_ElseIfPredicate.push_back(s_elseIfPredicate);

		i_startBody = i_endPredicate; //s_input.find(")");
		searchEndPosition(s_input, i_startBody, i_endBody);
		i_checkBracket = s_input.find("{",i_startBody);
		i_checkSemiColon = s_input.find(";",i_startBody);
		if (i_checkSemiColon > i_checkBracket && i_checkBracket != string::npos) {
			i_startBody = i_checkBracket;
			s_elseIfBody.assign(s_input,i_startBody+1,i_endBody-i_startBody-1);
		}
		else
			s_elseIfBody.assign(s_input,i_startBody+1,i_endBody-i_startBody);

		v_list_ElseIfBody.push_back(s_elseIfBody);
	}

	 // "else" statement
	string s_elseBody;
	if (b_elseExist) {
		i_startBody = s_input.find("else", i_endBody);
		searchEndPosition(s_input, i_startBody, i_endBody);
		i_checkBracket = s_input.find("{",i_startBody);
		i_checkSemiColon = s_input.find(";",i_startBody);
		if (i_checkSemiColon > i_checkBracket && i_checkBracket != string::npos) {
			i_startBody = i_checkBracket;
			s_elseBody.assign(s_input,i_startBody+1,i_endBody-i_startBody-1);
		}
		else {
			i_startBody = i_startBody + 4; // else consist of 4 letters
			s_elseBody.assign(s_input,i_startBody+1,i_endBody-i_startBody);
		}
	}

	string s_output;
	string s_elseIfEntry;
	s_output = "case " + s_entry + ": {\n"
		+ " if (" + s_ifPredicate + ")\n"
		+ "	" + s_switchVariable + " = " + s_thenEntry + ";\n";
		for (int i=0; i<v_list_ElseIfPredicate.size(); ++i) {
			s_elseIfEntry = intToString(v_list_elseIfEntry[i]);
			s_output +=  " else if (" + v_list_ElseIfPredicate[i] + ")\n"
			+ "	" + s_switchVariable + " = " + s_elseIfEntry + ";\n";
		}
	s_output += " else\n	"
		+ s_switchVariable + " = " + s_elseEntry + ";\n"
		+ " break;\n"
		+ " }\n";

	// write to file
	writeToBuffer(s_output);

	transform_block(s_ifBody,i_thenEntry, i_exit);

	for (int i=0; i<v_list_ElseIfBody.size(); ++i)
		transform_block(v_list_ElseIfBody[i],v_list_elseIfEntry[i], i_exit);

	if (b_elseExist)
		transform_block(s_elseBody,i_elseEntry, i_exit);
}

void AlgorithmControlFlow::transform_while(string &s_input, int i_entry, int i_exit)
{
	string s_switchVariable = m_levels.back().first;
	int i_bodyEntry = RANDOM_NUM;

	string s_entry, s_exit, s_bodyEntry;

	s_bodyEntry = intToString(i_bodyEntry);
	s_entry = intToString(i_entry);
	s_exit = intToString(i_exit);

	int i_startPredicate = 0, i_endPredicate = 0;
	string s_whilePredicate;
	i_startPredicate = s_input.find("(");
	searchEndBraces(s_input, i_startPredicate, i_endPredicate);
	s_whilePredicate.assign(s_input,i_startPredicate+1,i_endPredicate-i_startPredicate-1);

	int i_startBody = 0, i_endBody = 0;
	string s_whileBody;
	i_startBody = i_endPredicate;
	searchEndPosition(s_input, i_startBody, i_endBody);

	int i_checkBracket=0, i_checkSemiColon=0;
	i_checkBracket = s_input.find("{",i_startBody);
	i_checkSemiColon = s_input.find(";",i_startBody);
	if (i_checkSemiColon > i_checkBracket && i_checkBracket != string::npos) {
		i_startBody = i_checkBracket;
		s_whileBody.assign(s_input,i_startBody+1,i_endBody-i_startBody-1);
	}
	else
		s_whileBody.assign(s_input,i_startBody+1,i_endBody-i_startBody);

	string s_output;
	s_output = "case " + s_entry + ": {\n"
		+ " if (" + s_whilePredicate + ")\n"
		+ "	" + s_switchVariable + " = " + s_bodyEntry + ";\n"
		+ " else\n"
		+ "	" + s_switchVariable + " = " + s_exit + ";\n"
		+ " break;\n"
		+ " }\n";

	// write to file
	writeToBuffer(s_output);

	m_breaks.push_back(pair<int,int>(m_levels.size(),i_exit));
	m_continues.push_back(pair<int,int>(m_levels.size(),i_entry));
	transform_block(s_whileBody,i_bodyEntry, i_entry);
	m_breaks.pop_back();
	m_continues.pop_back();
}

void AlgorithmControlFlow::transform_switch(string &s_input, int i_entry, int i_exit)
{
	string s_switchVariable = m_levels.back().first;

	string s_entry, s_exit, s_bodyEntry;

	s_entry = intToString(i_entry);
	s_exit = intToString(i_exit);

	int i_startPredicate = 0, i_endPredicate = 0;
	string s_switchPredicate;
	i_startPredicate = s_input.find("(");
	searchEndBraces(s_input, i_startPredicate, i_endPredicate);
	s_switchPredicate.assign(s_input,i_startPredicate+1,i_endPredicate-i_startPredicate-1);

	int i_startBody = 0, i_endBody = 0;
	string s_switchBody, s_caseLabel;
	i_startBody = s_input.find("{", i_startPredicate);
	searchEndPosition(s_input, i_startBody, i_endBody);

    int i_casePos = 0, i_casePos2 = 0, i_symbol = 0;
    string s_casePredicate, s_caseBody, s_gotoLabel;
    while (s_input.find("case", i_casePos+1) != string::npos) {
        s_gotoLabel = rand_alnum_str(NUM_OF_RANDOM_CHAR);
        i_casePos = s_input.find("case", i_casePos+1);
        i_symbol =  s_input.find(":", i_casePos);
        s_casePredicate.assign(s_input, i_casePos, i_symbol-i_casePos+1);
        s_caseLabel += "	" + s_casePredicate + " goto " + s_gotoLabel +";\n";

        i_casePos2 = s_input.find("case", i_casePos+1);
        if (i_casePos2 == string::npos) {
            i_casePos2 = s_input.find("default", i_casePos+1);
            if (i_casePos2 == string::npos) {
                i_casePos2 = i_endBody;
            }
        }
        s_caseBody.assign(s_input, i_symbol+1, i_casePos2-i_symbol-1);
        s_switchBody += s_gotoLabel + ":	" + s_caseBody + "\n";
    }

    i_casePos = s_input.find("default", i_casePos);
    if (i_casePos != string::npos) {
        s_gotoLabel = rand_alnum_str(NUM_OF_RANDOM_CHAR);
        i_casePos2 = i_endBody;
        i_symbol =  s_input.find(":", i_casePos);
        s_caseBody.assign(s_input, i_symbol+1, i_casePos2-i_symbol-1);
        s_switchBody += s_gotoLabel + ":	" + s_caseBody + "\n";
        s_caseLabel += "	default : goto " + s_gotoLabel +";\n";
    }

	string s_output;
	s_output = "case " + s_entry + ": {\n"
		+ " switch (" + s_switchPredicate + ") {\n"
		+ s_caseLabel
		+ "	" + s_switchVariable + " = " + s_exit + ";\n"
		+ " break;\n"
		+ " }\n";

	// write to file
	writeToBuffer(s_output);

	int i_randNum = RANDOM_NUM;
	m_breaks.push_back(pair<int,int>(m_levels.size(),i_exit));
	transform_block(s_switchBody, i_randNum, i_exit);
	m_breaks.pop_back();
}

void AlgorithmControlFlow::transform_do(string &s_input, int i_entry, int i_exit)
{
	string s_switchVariable = m_levels.back().first;
	int i_testEntry = RANDOM_NUM;
	int i_bodyEntry = RANDOM_NUM;

	string s_entry, s_exit, s_bodyEntry, s_testEntry;

	s_testEntry = intToString(i_testEntry);
	s_bodyEntry = intToString(i_bodyEntry);
	s_entry = intToString(i_entry);
	s_exit = intToString(i_exit);

	int i_startBody = 0, i_endBody = 0;
	string s_doBody;
	i_startBody = s_input.find("{", 0);
	searchEndPosition(s_input, i_startBody, i_endBody);
	s_doBody.assign(s_input,i_startBody+1,i_endBody-i_startBody-1);

	int i_startPredicate = 0, i_endPredicate = 0;
	string s_doPredicate;
	i_startPredicate = s_input.find("(", i_endBody);
	i_endPredicate = s_input.rfind(")");
	s_doPredicate.assign(s_input,i_startPredicate+1,i_endPredicate-i_startPredicate-1);

	string s_output;
	s_output = "case " + s_testEntry + ": {\n"
		+ " if (" + s_doPredicate + ")\n"
		+ "	" + s_switchVariable + " = " + s_bodyEntry + ";\n"
		+ " else\n"
		+ "	" + s_switchVariable + " = " + s_exit + ";\n"
		+ " break;\n"
		+ " }\n";

	s_output += "case " + s_entry + ": {\n"
		+ "	" + s_switchVariable + " = " + s_bodyEntry + ";\n"
		+ " break;\n"
		+ " }\n";

	// write to file
	writeToBuffer(s_output);

	m_breaks.push_back(pair<int,int>(m_levels.size(),i_exit));
	m_continues.push_back(pair<int,int>(m_levels.size(),i_entry));
	transform_block(s_doBody,i_bodyEntry, i_testEntry);
	m_breaks.pop_back();
	m_continues.pop_back();
}

void AlgorithmControlFlow::transform_for(string &s_input, int i_entry, int i_exit)
{
	string s_switchVariable = m_levels.back().first;
	int i_testEntry = RANDOM_NUM;
	int i_incEntry = RANDOM_NUM;
	int i_bodyEntry = RANDOM_NUM;

	string s_entry, s_exit, s_bodyEntry, s_testEntry, s_incEntry;

	s_incEntry = intToString(i_incEntry);
	s_testEntry = intToString(i_testEntry);
	s_bodyEntry = intToString(i_bodyEntry);
	s_entry = intToString(i_entry);
	s_exit = intToString(i_exit);

	int i_startPredicate = 0, i_endPredicate = 0;
	string s_predicate;
	i_startPredicate = s_input.find("(");
	searchEndBraces(s_input, i_startPredicate, i_endPredicate);
	s_predicate.assign(s_input,i_startPredicate+1,i_endPredicate-i_startPredicate-1);

	int i_semicolonPos = 0, i_semicolonPos2 = 0;
	string s_forInit, s_forPredicate, s_forInc;
	i_semicolonPos = s_predicate.find(";");
	i_semicolonPos2 = s_predicate.find(";", i_semicolonPos+1);
	s_forInit.assign(s_predicate,0, i_semicolonPos);
	s_forPredicate.assign(s_predicate,i_semicolonPos+1, i_semicolonPos2-i_semicolonPos-1);
	s_forInc.assign(s_predicate,i_semicolonPos2+1, s_predicate.size());

	int i_startBody = 0, i_endBody = 0;
	string s_forBody;
	i_startBody = i_endPredicate;
	searchEndPosition(s_input, i_startBody, i_endBody);

	int i_checkBracket=0, i_checkSemiColon=0;
	i_checkBracket = s_input.find("{",i_startBody);
	i_checkSemiColon = s_input.find(";",i_startBody);
	if (i_checkSemiColon > i_checkBracket && i_checkBracket != string::npos) {
		i_startBody = i_checkBracket;
		s_forBody.assign(s_input,i_startBody+1,i_endBody-i_startBody-1);
	}
	else
		s_forBody.assign(s_input,i_startBody+1,i_endBody-i_startBody);

	string s_output;
	s_output = "case " + s_entry + ": {\n"
		+ " if (" + s_forInit + ")\n"
		+ "	" + s_switchVariable + " = " + s_testEntry + ";\n"
		+ " break;\n"
		+ " }\n";

	s_output += "case " + s_testEntry + ": {\n"
		+ " if (" + s_forPredicate + ")\n"
		+ "	" + s_switchVariable + " = " + s_bodyEntry + ";\n"
		+ " else\n"
		+ "	" + s_switchVariable + " = " + s_exit + ";\n"
		+ " break;\n"
		+ " }\n";

	s_output += "case " + s_incEntry + ": {\n"
		+ "	" + s_forInc + "\n"
		+ "	" + s_switchVariable + " = " + s_testEntry + ";\n"
		+ " break;\n"
		+ " }\n";

	// write to file
	writeToBuffer(s_output);

	m_breaks.push_back(pair<int,int>(m_levels.size(),i_exit));
	m_continues.push_back(pair<int,int>(m_levels.size(),i_entry));
	transform_block(s_forBody,i_bodyEntry, i_incEntry);
	m_breaks.pop_back();
	m_continues.pop_back();
}

void AlgorithmControlFlow::transform_try(string &s_input, int i_entry, int i_exit)
{
	string s_switchVariable = m_levels.back().first;

	string s_entry, s_exit, s_bodyEntry, s_testEntry, s_incEntry;

	s_entry = intToString(i_entry);
	s_exit = intToString(i_exit);

	int i_startBody = 0, i_endBody = 0;
	string s_tryBody;
	searchEndPosition(s_input, i_startBody, i_endBody);

	int i_checkBracket=0, i_checkSemiColon=0;
	i_checkBracket = s_input.find("{",i_startBody);
	i_checkSemiColon = s_input.find(";",i_startBody);
	if (i_checkSemiColon > i_checkBracket && i_checkBracket != string::npos) {
		i_startBody = i_checkBracket;
		s_tryBody.assign(s_input,i_startBody+1,i_endBody-i_startBody-1);
	}
	else
		s_tryBody.assign(s_input,i_startBody+1,i_endBody-i_startBody);

	string s_output;
	s_output = "case " + s_entry + ": {\n"
		+ " try {\n";
	writeToBuffer(s_output);
	flatten_block(s_tryBody);

	s_output = " }\n";
	writeToBuffer(s_output);

	while (s_input.find("(",i_endBody) != string::npos) {
		int i_startPredicate = 0, i_endPredicate = 0;
		string s_catchPredicate;
		i_startPredicate = s_input.find("(",i_endBody);
		searchEndBraces(s_input, i_startPredicate, i_endPredicate);
		s_catchPredicate.assign(s_input,i_startPredicate+1,i_endPredicate-i_startPredicate-1);

		string s_catchBody;
		i_startBody = i_endPredicate;
		searchEndPosition(s_input, i_startBody, i_endBody);

		i_checkBracket = s_input.find("{",i_startBody);
		i_checkSemiColon = s_input.find(";",i_startBody);
		if (i_checkSemiColon > i_checkBracket && i_checkBracket != string::npos) {
			i_startBody = i_checkBracket;
			s_catchBody.assign(s_input,i_startBody+1,i_endBody-i_startBody-1);
		}
		else
			s_catchBody.assign(s_input,i_startBody+1,i_endBody-i_startBody);


		s_output = "	catch (" + s_catchPredicate + ") {\n";
		writeToBuffer(s_output);
		flatten_block(s_catchBody);

		s_output = " }\n";
		writeToBuffer(s_output);
	}

	s_output = "	" + s_switchVariable + " = " + s_exit + ";\n"
		+ " break;\n"
		+ " }\n";

	// write to file
	writeToBuffer(s_output);
}

void AlgorithmControlFlow::transform_sequence(string &s_input, int i_entry, int i_exit)
{
	string s_output, s_tempInput, s_tempOutput, s_temp;
	string s_entry = intToString(i_entry);
	s_output = "case " + s_entry + ": {\n";

	// replace statement that is "break"
	// need to replace "break" first before replacing "continue"
	if (s_input.find("break") != string::npos) {
		string s_breakExit = intToString(m_breaks.back().second);
		s_temp = m_levels[m_breaks.back().first - 1].first + " = " + s_breakExit + ";\n";
		if (m_breaks.back().first != m_levels.size())
			s_temp += "goto " + m_levels[m_breaks.back().first-1].second + ";\n";
		else
			s_temp += "break;\n";

		stringToReplace(s_input, s_tempInput,"break", s_temp);
	}

	// replace for statement that is "continue"
	string s_copyofInput;
	if (s_tempInput.empty())
		s_tempInput = s_input;

	if (s_tempInput.find("continue") != string::npos) {
		s_copyofInput = s_tempInput;
		s_tempInput.clear();
		string s_continueEntry = intToString(m_continues.back().second);
		s_temp = m_levels[m_continues.back().first-1].first + " = " + s_continueEntry + ";\n";
		if (m_continues.back().first != m_levels.size())
			s_temp += "goto " + m_levels[m_continues.back().first-1].second + ";\n";
		else
			s_temp += "break;\n";

		stringToReplace(s_copyofInput, s_tempInput,"continue", s_temp);
	}

	if (!s_tempInput.empty())
		s_output += s_tempInput;
	else
		s_output += s_input;

	string s_exit = intToString(i_exit);
	s_output += m_levels.back().first + " = " + s_exit + ";\n"
		+ "break;\n"
		+ "}\n";

	writeToBuffer(s_output);
}

void AlgorithmControlFlow::findFunction(const string& s_input, int& i_startPosition, int &i_endPosition)
{
	i_startPosition = s_input.find("{", i_endPosition);
	searchEndPosition(s_input, i_startPosition, i_endPosition);
}

bool AlgorithmControlFlow::moveVariable(string& s_input, string& s_type, string& s_output)
{
	if (s_input.find(s_type) != string::npos) {
		int i_intPos, i_pointerPos, i_varPos, i_varEndPos;
		i_intPos = s_input.find(s_type);
		while (isalnum(s_input[i_intPos-1]) || isalnum(s_input[i_intPos+s_type.size()]) && i_intPos != string::npos) {
			i_intPos = s_input.find(s_type, i_intPos+1);
        }

        if (i_intPos == string::npos)
            return false;

        int i_semiColon = 0;
        i_semiColon  = s_input.find(";",i_intPos);

        string s_variable;
        s_variable.assign(s_input, i_intPos, i_semiColon-i_intPos+1);
        s_output.insert(0,s_variable);
        s_input.erase(i_intPos, s_variable.size());
        return true;
	}
}

void AlgorithmControlFlow::stringToReplace(string& s_input, string& s_output, string s_toReplace, string& s_replaceWith)
{
	int i_start=0, i_end=0;
	i_end = s_input.find(s_toReplace,i_start);

	while (i_end != string::npos) {
		s_output.append(s_input,i_start, i_end-i_start);
		i_start = i_end + 1;

		s_output.append(s_replaceWith);
		if (i_start != string::npos && i_end != string::npos)
			i_end = s_input.find(";",i_end);

		if (i_end != string::npos) {
			i_start = i_end+1;
			i_end = s_input.find(s_toReplace,i_start);
		}
	}

	s_output.append(s_input,i_start,s_input.size());
}

// search for end brackets
void AlgorithmControlFlow::searchEndBracket(const string& s_input, int i_startPosition, int &i_endPosition)
{
		int i_posOfOtherBracket=i_startPosition,
			i_posOfEndBracket=0;

		i_posOfOtherBracket = s_input.find("{",i_startPosition);
		i_posOfEndBracket = s_input.find("}",i_startPosition);
		while (s_input.find("{",i_posOfOtherBracket + 1) != string::npos){
			i_posOfOtherBracket = s_input.find("{",i_posOfOtherBracket + 1);
			if (i_posOfOtherBracket > i_posOfEndBracket)
				break;

			i_posOfEndBracket = s_input.find("}",i_posOfEndBracket + 1);
		}
		i_endPosition  = i_posOfEndBracket;
}

void AlgorithmControlFlow::searchEndBraces(const string& s_input, int i_startPosition, int &i_endPosition)
{
		int i_posOfOtherBracket=i_startPosition,
			i_posOfEndBracket=0;

		i_posOfOtherBracket = s_input.find("(",i_startPosition);
		i_posOfEndBracket = s_input.find(")",i_startPosition);
		while (s_input.find("(",i_posOfOtherBracket + 1) != string::npos){
			i_posOfOtherBracket = s_input.find("(",i_posOfOtherBracket + 1);
			if (i_posOfOtherBracket > i_posOfEndBracket)
				break;

			i_posOfEndBracket = s_input.find(")",i_posOfEndBracket + 1);
		}
		i_endPosition  = i_posOfEndBracket;
}

// search if semicolon or bracket is end position
void AlgorithmControlFlow::searchEndPosition(const string& s_input, int i_startPosition, int &i_endPosition)
{

	int i_posSemiColon = 0, i_posOpenBracket = 0;
	i_posSemiColon = s_input.find(";", i_startPosition);
	i_posOpenBracket = s_input.find("{", i_startPosition);

	if (i_posOpenBracket != string::npos) {
		if ( i_posSemiColon < i_posOpenBracket ) {
			i_endPosition = i_posSemiColon;
		}

		else if ( i_posSemiColon > i_posOpenBracket ) {
			searchEndBracket(s_input, i_startPosition, i_endPosition);
		}
	}
	else if (i_posSemiColon != string::npos)
		i_endPosition = i_posSemiColon;
}

// search if there is "else if" or "else"
void AlgorithmControlFlow::searchEndOfElseIf(const string& s_input, int i_startPosition, int& i_endPosition, int& i_numElseIf, bool& b_elseExist){
	int i_if = 0, i_else = 0, i_elseIf = i_startPosition;
	int i_tempNumElseIf = 0, i_tempPosition = 0;;

	//search for another "if" starting from end position
	if (s_input.find("if", i_startPosition) != string::npos) {
		i_if = s_input.find("if", i_startPosition);
		// make sure the "if" match the whole word and not inbetween words
		// and also will not match "else if"
		while (s_input[i_if-5] == 'e' && s_input[i_if-4] == 'l' &&
			s_input[i_if-3] == 's' && s_input[i_if-2] == 'e' ||
			isalnum(s_input[i_if-1]) || isalnum(s_input[i_if+2])
			&& i_if != string::npos)
		{
			i_if = s_input.find("if", i_if+1);

		}
		if (i_if == string::npos)
			i_if = s_input.size();
	}
	else {// cannot find "if"
		i_if = s_input.size();
	}

	int i_pos = s_input.find("else if", i_startPosition);

	if (i_pos < i_if && i_pos != string::npos) {
		i_elseIf = s_input.find("else if", i_startPosition);

		while (i_elseIf < i_if && i_elseIf != string::npos) {
			i_tempPosition = i_elseIf;
			searchEndPosition(s_input, i_tempPosition, i_elseIf);

			++i_tempNumElseIf;
			if (s_input.find("else if", i_elseIf+1) == string::npos)
				break;
		}
		if (i_elseIf > i_if) {
			i_tempPosition = i_elseIf;
			searchEndOfElseIf(s_input, i_tempPosition,i_elseIf,i_tempNumElseIf,b_elseExist);
			i_numElseIf = i_tempNumElseIf;
			i_endPosition = i_elseIf;
			return;
		}

		i_numElseIf = i_tempNumElseIf;
		i_endPosition = i_elseIf;
	}

	//search inbetween for "else"
	if (s_input.find("else", i_elseIf) != string::npos){
		i_else = s_input.find("else", i_elseIf);
		if (i_else < i_if) {
			//mean "else" exist and it does not matter where the another "if" is
			//ending is after "else"
			//find that position then pass it back through reference
			searchEndPosition(s_input, i_elseIf+1, i_else);
			b_elseExist = true;
			i_endPosition = i_else;
			return;
		}
	}

	if (i_tempNumElseIf == 0 && !b_elseExist) {// cannot find "else if" or "else"
		i_endPosition = i_startPosition;
		return;
	}
}
