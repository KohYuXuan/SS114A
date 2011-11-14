//#include "Engine.h"

#include "ReverseAlgorithm.h"

void ReverseAlgorithm::setUpMapping(const string& s_input, map<string,string>& m_output)
{
        string s_string;
        size_t i_firstDelim = 0, i_secondDelim = 0;
        vector<string> v_string;
m_output.clear();
        i_firstDelim = s_input.find_first_of(';');
        i_secondDelim = s_input.find_first_of(';',i_firstDelim+1);
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

map<string,string>::iterator m_iter;
for ( m_iter=m_output.begin() ; m_iter != m_output.end(); m_iter++ )
        cout << (*m_iter).first << " => " << (*m_iter).second << endl;
}

void ReverseAlgorithm::reverseModiftString(string& output, const string& str, const vector<string>& v,map<string,string>& mymap)
{
    cout << "ReverseAlgorithm::reverseModiftString : " << endl;
    locale loc;
        size_t found=0;
        string s_temp, s_random;
        map<string,string>::iterator it;

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
cout << "ReverseAlgorithm::reverseModiftString :   i :" << i << endl;
cout << "ReverseAlgorithm::reverseModiftString :   found :" << found << endl;
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
cout << "Algorithm2::modiftString : s_temp : " << s_temp << endl;















if(!s_temp.empty())

                        it=mymap.find(s_temp);
                        if( it != mymap.end())
                                s_random = (*it).second;cout << (*it).second << endl;
cout << s_temp << endl;

if(!s_random.empty())
                             output.append(s_random);

                        found += s_temp.size();
                        i = found-1;
                        ++i_vector;
                }
        }
        cout << output << endl;
}

void ReverseAlgorithm::formatFile(const string& s_input, string& s_output)
{

    string s_tabToAppend;
    int i_tabToAppend = 0;

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
                                s_output.append("\t");
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







































void ReverseAlgorithm::addWordsToList(const string& str,vector<string>& v)
{
        locale loc;
        string s_temp;
        //bool s_found = false;
        //int numOfLetter;
        for(int i=0; i<str.size(); ++i)
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






void ReverseAlgorithm::modiftString(string& output, const string& str, const vector<string>& v,map<string,string>& mymap)
{	locale loc;
        size_t found=0;
        string s_temp, s_random;
        map<string,string>::iterator it;
        set<string>::iterator set_it;
        int i_vector=0;
        bool b_eof = false;
        output.clear();


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



void ReverseAlgorithm::removeWordsFromList(map<string,string> m_input,vector<string>& vec)
{
cout << "ReverseAlgorithm::removeWordsFromList: "  << endl;
set<string> set_input;
map<string,string>::iterator it;
for ( it=m_input.begin() ; it != m_input.end(); it++ ) {
    set_input.insert((*it).first);
}

set<string>::iterator s_it;
for ( s_it=set_input.begin() ; s_it != set_input.end(); s_it++ ) {
   cout << "ReverseAlgorithm::set_input  : " << *s_it << endl;
}

        string s_temp;
        vector<string> v_output;
        set<string>::iterator set_it;
        for(int i=0; i<vec.size(); ++i)
        {
                        s_temp = vec[i];
                if( (set_it=set_input.find(s_temp)) != set_input.end() ){
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

void ReverseAlgorithm::findWordsFromList(set<string>& set_input, vector<string>& v_input, vector<string>& v_output)
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

void ReverseAlgorithm::skipQuote(const string& s_input, int& i_skip, char c_quote) // c_quote can be double or single quote
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


void ReverseAlgorithm::convertQuotedStringToHex(string& s_searchString)
{
        size_t i_firstQuote;
        size_t i_secondQuote;
        size_t i_sizeOfQuotedString;
        vector<string> v_listOfQuotedString;
        string s_temp;
        string s_hex;
        int i_sizeToRepalce;
        int i_posToReplace;

cout << "s_searchString : " << s_searchString << endl;
	// find and skip header file
	int i_headerPos=0;
	int i_newlinePos=0;
	i_headerPos = s_searchString.rfind("#include");
	if(i_headerPos!=string::npos) {
		i_newlinePos = s_searchString.find("\n", i_headerPos);
	}	

cout<<"i_headerPos : " << i_headerPos <<endl;
cout<<"i_newlinePos : " << i_newlinePos <<endl;
//i_firstQuote = i_newlinePos;



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
                                hexToString(s_temp, s_hex);
                                s_searchString.replace ( i_posToReplace, i_sizeToRepalce, s_hex, 0, s_hex.size() );

                        }
                }
                cout << s_searchString << endl;

}

void ReverseAlgorithm::hexToString(const string& s, string& output)
{

        output.clear();
        size_t found, found2;
        string temp;
        stringstream ss;
        int i_temp;
        found=s.find_first_not_of("\\x");
        if(found != string::npos)
                found2=s.find("\\x",found);
        while (found!=string::npos) {
                temp.clear();
                ss.str("");
                ss.clear();
		

                if(found2 != string::npos) {
                        temp.append(s,found, found2-found);
cout << temp << endl;

                        ss << hex <<temp;
                        ss >> i_temp;
                        //istringstream(temp) >> i_temp;
cout << static_cast<char>(i_temp)<<endl;
                        output.append(1, (char) i_temp);



			if(temp.find(".")!=string::npos)
				output.append(".");
			if(temp.find("\n")!=string::npos)
				output.append("\\n");
			if(temp.find("t")!=string::npos)
				output.append("\\t");





                        found=s.find_first_not_of("\\x",found2);
                        found2=s.find("\\x",found);
                }
                else {
                        temp.append(s,found, s.size()-found);
cout << temp << endl;
                        ss << hex <<temp;
                        ss >> i_temp;
                        //istringstream(temp) >> i_temp;
cout << static_cast<char>(i_temp)<<endl;
                        output.append(1, (char) i_temp);



			if(temp.find(".")!=string::npos)
				output.append(".");
			if(temp.find("n")!=string::npos)
				output.append("\\n");
			if(temp.find("t")!=string::npos)
				output.append("\\t");





                        break;
                }

        }
        cout  << "hexToString : " << output << endl;
        //return s;
}



