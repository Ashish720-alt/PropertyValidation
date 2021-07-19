#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <iostream>
#include <stack>  
#include <sstream> 
#include <map> 
#include <vector> 
#include <set>
#include <iterator> 
#include <utility>
#include <queue>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <ctype.h>
#include <string>

// If get a segfault with this file, then increase the value of macro LINESIZE in macros.hpp

#define LINESIZE 300000

using namespace std;

bool removeUptodate ( char* s) ;
void removeDollar(char *s);
string removeTypes ( string s);
void append(char subject[], const char insert[], int pos);
void removedoublebrace (string& s);
void removeemptybrace (string& s) ;
void extractvaluedict ( string& s) ;
void extractstreamdict ( string& s) ;
string constructID (string& s );
char getignoreWS (string& s, long& pos);
string readstring (string& s, long& pos);
string read_unquoted_string (string& s, long& pos);
string readValueArray (string& s, long& pos);
string readValueDict (string& s, long& pos);
string readValueInt (string& s, long& pos);
char peekignoreWS (string& s, long& pos);
long stringToInt (string s);
long findPosInVectors ( vector <pair<long, long> > vp, pair<long, long> p );
string indent (string& s);
string changerefToID (string& s, vector <pair<long, long> > vp);
bool checkref(string s, long& pos, vector <pair<long, long> > vp, long& id ) ;
template <class T> inline string to_string (const T& t);
void fixbrokendictionary (string& s);
string erase_word (string s, const string e) ;
string read_till_delim_respect_paranthesis (string s, long& pos, set<char> delims  ) ;
string unpack_dictionaries( string s);
vector< pair<string, string> > unpack_dictionary (string s, long pos, string prefix_to_keynames) ;
void remove_random_dot_after_numbers(string& s) ;
string trimWS (string s);

int main( int argc, char *argv[] )  {

    vector < pair<long, long> > idlist;
    
    if ( argc != 2) {
        fprintf(stderr, "Incorrect arguments: Give path to Daedalus output file!\n"); 
    }

    ifstream t(argv[1]);
    string doc, doc1;

    stringstream buffer;
    buffer << t.rdbuf();
    doc = buffer.str();

    doc.erase(0, sizeof("Up to date"));

    // Remove types
    doc = removeTypes( doc); 
    // Removes doublebraces created by removing types!
    removedoublebrace(doc);
    // Removes empty braces;
    removeemptybrace(doc);
    // Add Key Quotes



    // Extract fields from "value" dictionary if field is a dictionary itself only!
    // Extract value dictionary!
    extractvaluedict(doc);   

    // Extract fields from "stream" dictionary if field is a dictionary itself only!
    extractstreamdict(doc);

    // Add id tag, add key quotes, change refs to refer to ID tags, and indent!
    doc1 = constructID(doc);        



    doc1 = unpack_dictionaries(doc1);

    remove_random_dot_after_numbers(doc1);

    string s = argv[1];
    s = s.substr(0, s.find("."));
    s.append(".json");
    FILE *outfile = fopen ( s.c_str() , "w");     
    fprintf(outfile, "%s", doc1.c_str()) ; 
    fclose(outfile);
    
    return 0;
}

void remove_random_dot_after_numbers(string& s) {
    for (long i = 0; i < s.size() ; i ++) {
        if ( !isdigit(s[i]) )
            continue;
        for( ; i < s.size() && isdigit(s[i]) ; i++ );
        if ( i <s.size() && s[i] == '.' && (i == s.size() - 1 || !isdigit(s[i+1]) ) )
            s[i] = ' ';
    }
    return;
}




string unpack_dictionaries( string s) {
    string rv;
    vector< pair<string, string> > dict;
    vector< pair<string, string> > dict_helper;
    vector< pair<string, string> > dict_helper_helper;
    bool is_dict;
    bool is_array_of_dict;
    if (s[0] != '[')
        return "";
    rv.append("[\n");
    for (long i = 1; s[i] != ']'; i++) {
        if (s[i] == ' ' || s[i] == '\n' || s[i] == ',')
            continue;
        else if (s[i] == '{') {
            dict.clear();
            rv.append("{\n");
            for (; s[i] != '}' ; )
            {
                string key = readstring(s, i);
                i++;
                while (s[i] == ' ' || s[i] == '\n' || s[i] == ':')
                    i++;
                char delimiters[2] = {',' , '}'};
                set<char> delims (delimiters, delimiters + 2) ;
                string value = read_till_delim_respect_paranthesis(s, i, delims);
                key = trimWS(key);
                value = trimWS(value);
                is_dict = (value[0] == '{');
                is_array_of_dict = (value[0] == '[');
                if (is_array_of_dict) {
                    long k;
                    for (k = 1; k < value.size() && (value[k] == ' ' || value[k] == '\n') ; k++ ) ;
                    is_array_of_dict = (value[k] == '{');
                }
                if (is_dict) {// Is a dictionary 
                    dict_helper.clear();
                    dict_helper = unpack_dictionary(value, 0, key.substr(1, key.size() - 2) ) ;   
                }   
                if (is_array_of_dict) {  
                    dict_helper.clear();
                    char new_delimiters[2] = {',' , ']'};
                    set<char> new_delims (new_delimiters, new_delimiters + 2) ;
                    for (long j = 1, N = 0; value[j] != ']' ; N++) {
                        string value_dict = read_till_delim_respect_paranthesis(value, j, new_delims);
                        if (value[j] == ',')
                            j++;
                        dict_helper_helper.clear();
                        dict_helper_helper = unpack_dictionary( trimWS(value_dict), 0, key.substr(1, key.size() - 2) + "." + trimWS(to_string(N)) + "." );
                        for(long l = 0 ; l < dict_helper_helper.size(); l++)
                            dict_helper.push_back(dict_helper_helper[l]);
                    }
                }
                if (is_dict || is_array_of_dict) { 
                    for (long k = 0; k < dict_helper.size(); k++) {
                        pair<string, string> temp = dict_helper[k];
                        temp.first = "\"" + temp.first + "\"";
                        dict.push_back( temp );
                    }
                }
                else
                    dict.push_back(  pair<string, string>(key, value) );
                // s[i] == ',' || s[i] == '}'
            }
            for (long k = 0; k < dict.size(); k++) { 
                pair<string, string> it = dict[k];
                rv.append("\t");
                if (k != 0)
                    rv.append(",");                
                rv.append("\t" + it.first + " : " + it.second + "\n");

            }
            rv.append("}\n");
        }
        else    
            return "";
        long j;
        for (j = i + 1; j < s.size() && (s[j] == ' ' || s[j] == '\n' || s[j] == '\r') ; j++ ) ;
        if (s[j] != ']')
            rv.append(",\n");

    }
    rv.append("]\n");
    return rv;

}


vector< pair<string, string> > unpack_dictionary (string s, long pos, string prefix_to_keynames) {
    long i = pos;
    vector< pair<string, string> > rv;
    vector< pair<string, string> > dict_helper2;
    bool is_dict;
    if (s[i] != '{')
        return vector< pair<string, string> > ();
    for (; s[i] != '}' ; )
    {
        string key = prefix_to_keynames + "." + read_unquoted_string(s, i);
        i++;
        while (s[i] == ' ' || s[i] == '\n' || s[i] == ':')
            i++;
        char delimiters[2] = {',' , '}'};
        set<char> delims (delimiters, delimiters + 2) ;
        string value = trimWS( read_till_delim_respect_paranthesis(s, i, delims) );
        is_dict = (value[0] == '{');
        if (is_dict) {// Is a dictionary 
            dict_helper2.clear();
            dict_helper2 = unpack_dictionary(value, 0, key ) ;   
        }
        else if (value == "true")
            value = "\"" + value + "\"";
        else if (value == "false")
            value = "\"" + value + "\"";
        if (!is_dict)
            rv.push_back(  pair<string, string>(key, value) );
        else    {
            for (long k = 0; k < dict_helper2.size(); k++) {
                rv.push_back( dict_helper2[k] );
            }
        }
        // s[i] == ',' || s[i] == '}'

    }
    return rv;    
}




string read_till_delim_respect_paranthesis (string s, long& pos, set<char> delims  ) {
    bool is_string = false;
    long ob = 0;
    long cb = 0;
    long sb = 0;
    long init_pos = pos;
    long i;
    for (i = pos; !( delims.find(s[i]) != delims.end() && ob == 0 && cb == 0 && sb == 0 && is_string == false) ; i++) {
        if (s[i] == '\"' && s[i-1] != '\\')
            is_string = !is_string;
        else if (s[i] == '[' && !is_string)
            sb++;
        else if (s[i] == ']' && !is_string)
            sb--;
        else if (s[i] == '(' && !is_string)
            ob++;
        else if (s[i] == ')' && !is_string)
            ob--;      
        else if (s[i] == '{' && !is_string)
            cb++;
        else if (s[i] == '}' && !is_string)
            cb--;             
    }
    pos = i;
    return s.substr(init_pos, pos - init_pos);
}





bool removeUptodate ( char* s) 
{
    if ( !strcmp(s, "Up to date\n"))
        return true;
    return false;
}


string removeTypes (string s) { // Add the quotes to the keystrings!
    string retval = s;
    retval = erase_word(retval, "$dict:");
    retval = erase_word(retval, "$name:");
    retval = erase_word(retval, "$ref:");
    return retval;
}


string erase_word (string s, const string e) {
    string rv = s;
    size_t pos = rv.find(e, 0 );
    while (pos != string::npos) {
        rv.erase(pos, e.size() );
        pos = rv.find(e, pos);
    }
    return rv;
}


void append(char subject[], const char insert[], int pos) {
    char buf[3 * LINESIZE] = {};
    strncpy(buf, subject, pos); 
    int len = strlen(buf);
    strcpy(buf+len, insert); 
    len += strlen(insert);  
    strcpy(buf+len, subject+pos);
    strcpy(subject, buf);   
}

void removedoublebrace (string& s) {
    long brace = 0;
    bool doublebrace = 0;
    stack<long> bracepos;

    for (long i = 0; i < s.length(); i++) { 
        if (s[i] == '{' && doublebrace == 0)  {   
            doublebrace = 1;
            brace ++;
        }
        else if (s[i] == '{' && doublebrace == 1) {
            s[i] = ' ';
            doublebrace = 1;
            bracepos.push(brace);
            brace ++;
        }
        else if ( !( s[i] == '}' || s[i] == '{' || s[i] == ' ' || s[i] == '\n') && doublebrace == 1) {
            doublebrace = 0;
        }
        else if ( s[i] == '}' && doublebrace == 1) {
            doublebrace = 0;
            brace --;
            if (  !bracepos.empty()  && bracepos.top() == brace) {
                bracepos.pop();
                s[i] = ' ';
            }
        }
        else if ( s[i] == '}' && doublebrace == 0) {
            brace --;
            if (  !bracepos.empty()  && bracepos.top() == brace) {
                bracepos.pop();
                s[i] = ' ';
            }
        }
        
    } 

    return;
}

void removeemptybrace (string& s) {
    bool brace = 0;
    long pos = -1;
    for (long i = 0; i < s.length(); i++ ) {
        if ( s[i] == '{') {
            brace = 1;
            pos = i;
        }
        else if ( s[i] != '{' && brace == 1) {
            
            if ( s[i] == '}') {
                s[i] = ' ';
                s[pos] = ' ';
                pos = -1;
                brace = 0;
            }
            else if ( s[i] == ' ' || s[i] == '\n') { }   
            else {
                brace = 0;
                pos = -1;
            }         
        }
        else {  }

    }
    return;
}

void extractvaluedict ( string& s) {
    bool foundvalue = 0;
    long pos1 = -1;
    long pos2 = -1;
    long pos3 = -1;
    long pos4 = -1;
    long cb, sb;


    for ( long i = 0; i < s.length(); i++ ) {
        if (s[i] == 'v') {
            pos1 = i;
            i++;
            if (s[i] != 'a') {
                for ( ; s[i] != '\"'; i++ ) {
                }
                    continue;
                
            }
            i++;
            if (s[i] != 'l') {
                for ( ; s[i] != '\"'; i++ );
                continue;
            }
            i++;
            if (s[i] != 'u') {
                for ( ; s[i] != '\"'; i++ );
                continue;
            }
            i++;
            if (s[i] != 'e') {
                for ( ; s[i] != '\"'; i++ );
                continue;
            }
            i++;
            
            while (s[i] == ' ' || s[i] == '\n') {
                i++;
            }
            if (s[i] != ':') {
                continue;
            }
            pos4 = i;
            i++;
            while (s[i] == ' ' || s[i] == '\n') {
                i++;
            }
            if (s[i] != '{') {
                if (s[i] == '\"') // Happens when value is a string, don't extract!
                    i--;
                continue;
            }                        
            pos2 = i;   
            cb = 1; 
            sb = 0;
            bool brokendictcolon = true;
            bool brokendictcomma = true;
            

            bool is_string = false;
            bool is_previous_slash = false;
            while ( !(cb == 0 && sb == 0) ) {
                i++;
                if (s[i] == '\"' && s[i-1]!= '/' ) 
                    is_string = !is_string;
                else if (s[i] == '{' && !is_string)
                    cb++;
                else if (s[i] == '}' && !is_string)
                    cb--;   
                else if (s[i] == '['  && !is_string)
                    sb++;
                else if (s[i] == ']'  && !is_string)
                    sb--;                 
                else if (s[i] == ':'  && !is_string)
                    brokendictcolon = false;
                else if (s[i] == ','  && !is_string)
                    brokendictcomma = false;
                
                   

            }
            

            if (brokendictcolon && brokendictcomma)
                continue;
            pos3 = i;
            s[pos2] = ' ';
            s[pos3] = ' ';
            for (long j = 0; j <= 6; j++)
                s[pos1 + j] = ' ';
            s[pos4] = ' ';
        }
    }
    
    return;
}

void extractstreamdict ( string& s) {
    bool foundvalue = 0;
    long pos1 = -1;
    long pos2 = -1;
    long pos3 = -1;
    long pos4 = -1;
    long cb, sb;

    for ( long i = 0; i < s.length(); i++ ) {
        if (s[i] == 's') {
            pos1 = i;
            i++;
            if (s[i] != 't') {
                for ( ; s[i] != '\"'; i++ );
                continue;
            }
            i++;
            if (s[i] != 'r') {
                for ( ; s[i] != '\"'; i++ );
                continue;
            }
            i++;
            if (s[i] != 'e') {
                for ( ; s[i] != '\"'; i++ );
                continue;
            }
            i++;
            if (s[i] != 'a') {
                for ( ; s[i] != '\"'; i++ );
                continue;
            }
            i++;
            if (s[i] != 'm') {
                for ( ; s[i] != '\"'; i++ );
                continue;
            }
            i++;
            
            while (s[i] == ' ' || s[i] == '\n') {
                i++;
            }
            if (s[i] != ':') {
                continue;
            }
            pos4 = i;
            i++;
            while (s[i] == ' ' || s[i] == '\n') {
                i++;
            }
            if (s[i] != '{') {
                continue;
            }                        
            pos2 = i;   
            cb = 1; 
            sb = 0;
            while ( !(cb == 0 && sb == 0) ) {
                i++;
                if (s[i] == '{')
                    cb++;
                if (s[i] == '}')
                    cb--;   
                if (s[i] == '[')
                    sb++;
                if (s[i] == ']')
                    sb--;                 
            }
            pos3 = i;
            s[pos2] = ' ';
            s[pos3] = ' ';
            for (long j = 0; j <= 7; j++)
                s[pos1 + j] = ' ';
            s[pos4] = ' ';
        }
    }
    
    return;
}

string constructID (string& s ) {

     // replace references by ID;
    string key;
    string valtemp;
    string value;
    char id, id1, id2;
    long pos = -1;
    long N = 1;
    bool writeid = 1;
    long obj, gen;
    vector <pair<long, long> > vp; 
    string doctemp, doctemp2;
    bool first = false;

    id2 = getignoreWS( s, pos ); // id2 = '['
    doctemp.append("[\n");
    while (id2 != ']') {
        if (first) {
            doctemp.append(",\n");
        }
        else {
            first = true;
        }
        id1 = getignoreWS( s, pos); // id1 = '{'
        while (id1 != '}') {  
            key = read_unquoted_string(s, pos);
            key = "\"" + key + "\"";
            getignoreWS(s, pos ); // expect s[pos] =':'
            id = peekignoreWS(s, pos);
            if (id == '\"' ) {
                valtemp = readstring(s, pos);
            }
            else if (id == '[') {
                valtemp = readValueArray(s, pos);
                 fixbrokendictionary(valtemp);
            }
            else if (id == '{') {
                valtemp = readValueDict(s, pos);
                 fixbrokendictionary(valtemp);
            }
            else if ( isdigit(id) || id == '-') {
                valtemp = readValueInt(s, pos);
            }
            else if (id == '}' || id == ',') { // Value is an empty string!
                valtemp = "\"\"";
            }
            else if (id == 'f' ) {  // Value is true or false.
                pos += 5;
                valtemp = "\"False\""; 
            }
            else if (id == 't') {
                pos += 4;
                valtemp = "\"True\"";
            }
            id1 = getignoreWS(s, pos);  // expect ',' or '}'  
            if (writeid) {
                doctemp.append("  {\n");
                doctemp.append("    \"Id\": " + to_string(N) + "\n") ;
                // doctemp.append(  );
                // doctemp.append( 1, '\n' ) ;
                N++;
                writeid = 0;
            } // Comment out this whole if block
            if (key == "\"obj\"") {
                obj = stringToInt( valtemp);
            }
            if (key == "\"gen\"") {
                gen = stringToInt( valtemp);
            }            
            value = indent (valtemp);
            doctemp.append("  , ");
            doctemp.append(key);
            doctemp.append(": ");
            doctemp.append(value);
            doctemp.append(1, '\n');
        }
        vp.push_back( make_pair(obj, gen) );
        doctemp.append("  }\n");
        writeid = 1;
        id2 = getignoreWS(s, pos);
    }
    doctemp.append("]\n");
    doctemp2 = changerefToID(doctemp, vp); // Only indentation, comment this line!
    return doctemp2; 

}

char peekignoreWS (string& s, long& pos) {
    pos++;
    while ( s[pos] == ' ' || s[pos] == '\n') {
        pos++; 
    }
    pos--;
    return s[pos + 1];
}

char getignoreWS (string& s, long& pos) {
    pos++;
    while ( s[pos] == ' ' || s[pos] == '\n') {
        pos++; 
    }
    return s[pos];
}


string read_unquoted_string (string& s, long& pos) { // reads quoted string, and sets pos after final \"
    long init_pos;    
    getignoreWS( s, pos );

    init_pos = pos;
    pos++;
    while ( !(s[pos] == ' ' || s[pos] == '\n' || s[pos] == ':' ) )
        pos++;
    long final_pos = pos;
    string rv = s.substr(init_pos, final_pos - init_pos);
    pos--;
    return rv;
}

// string read_unquoted_string_with_colon (string& s, long& pos) { // reads quoted string, and sets pos after final \"
//     long init_pos;    

//     while (s[pos] == ' ' || s[pos] == '\n')
//         pos++;

//     init_pos = pos;
//     pos++;
//     while ( !(s[pos] == ' ' || s[pos] == '\n') )
//         pos++;
//     long final_pos = pos;
//     string rv = s.substr(init_pos, final_pos - init_pos);
//     pos--;
//     return rv;
// }




string readstring (string& s, long& pos) { // reads quoted string, and sets pos after final \"
    long init_pos;    
    getignoreWS( s, pos );

    init_pos = pos;
    pos++;
    while ( !(s[pos] == '\"' && s[pos-1] != '\\') )
        pos++;
    long final_pos = pos;
    string rv = s.substr(init_pos, final_pos - init_pos + 1);
    return rv;
}


    // char c = getignoreWS( s, pos);
    // string text(1, c);
    // c = 'a';
    // while ( c != '\"') {
    //     pos++;
    //     c = s[pos];
    //     text.append(1 ,c);
    // }
    // return text;


string readValueArray (string& s, long& pos) {
    char c = getignoreWS( s, pos);
    long cb = 0;
    long sb = 1;
    string value(1, c);
    c = 'a';
    while ( c != ']' || cb != 0 || sb != 0) {
        pos++;
        c = s[pos];
        if (c == '[')
            sb++;
        else if (c == ']')
            sb--;
        else if (c == '{')
            cb++;
        else if (c == '}')
            cb--;
        value.append(1 ,c);
    }
    return value;
}

string readValueDict (string& s, long& pos) {
    char c = getignoreWS( s, pos);
    long cb = 1;
    long sb = 0;
    string value(1, c);
    c = 'a';
    while ( c != '}' || cb != 0 || sb != 0) {
        pos++;
        c = s[pos];
        if (c == '[')
            sb++;
        else if (c == ']')
            sb--;
        else if (c == '{')
            cb++;
        else if (c == '}')
            cb--;
        value.append(1 ,c);
    }
    return value;
}

string readValueInt (string& s, long& pos) {
    bool neg = false;
    char c = getignoreWS( s, pos);
    string value;
    if (c == '-') {
        neg = true;
        value.append(1, c);
        c = '0';
    }
    while ( isdigit(c) || c == '.') { // Deadalus output has a '.' at the end of some integers.
        if (!neg) {
            value.append(1 ,c);
        }
        else {
            neg = false;
        }

        pos++;
        c = s[pos];
    }
    if (value[value.size() - 1] == '.' )
        value.erase(value.size() - 1,1) ;
    pos--;
    return value;
}

long stringToInt (string s) {
    stringstream temp;
    long num;
    temp.str(s);
    temp >> num;
    return num;
}

long findPosInVectors ( vector <pair<long, long> > vp, pair<long, long> p ) {
    for (long pos = 0 ; pos < vp.size() ; pos++) {
        if ( (vp[pos]).first == p.first && (vp[pos]).second == p.second ) {
            return pos;
        }
    }
    return -1;
}

string indent (string& s) {
    string s2, s3;
    long pos;

    for (long i = 0; i < s.length(); i++) {
        if (s[i] == '\n') 
            s[i] = ' ';
    }

    for  (long i = 0; i < s.length(); i++) {
        if (s[i] != ' ') {
            s2.append(1, s[i]);
            continue;
        }
        else {
            for ( ; s[i] == ' '; i++) ;
            i--;
            s2.append(1, ' ');
            continue;
        }
    }

    for (long i = 0; i < s2.length() ; i++) {
        if (s2[i] == ':') {
            s3.append(1, s2[i]);
            pos = i;
            i++;
            for ( ; s2[i] == ' ' ; i++);
            if (s2[i] == ',') {
                s3.append(" \"\" ,");
                continue;
            }
            i = pos;
            continue;
        }
        s3.append(1, s2[i]);
    }

    return s3;
}

string changerefToID (string& s, vector <pair<long, long> > vp) {
    long id = 0;
    string rv;

    for (long i = 0; i < s.length(); i++) {
        long j = i;
        if ( checkref(s, j, vp, id) ) {
                rv.append( to_string(id) ) ;
                i = j - 1;
                continue;
        }
        rv.append( 1, s[i] );
    }

    return rv;
}


bool checkref(string s, long& pos, vector <pair<long, long> > vp, long& id ) {
    if (pos < 0)
        return false; 
    long num1, num2;
    id = 0;
    if (s.substr(pos, 7 ) != "{ obj: " ) {
        return false;    
    }
    pos += 6;

    num1 = stringToInt (readValueInt( s, pos));
    pos++;

    if (s.substr(pos, 7) != ", gen: " ) {
        return false;    
    }
    pos += 6;
    num2 = stringToInt (readValueInt( s, pos));
    pos++;
    while (s[pos] == ' ' || s[pos] == '\n')
        pos++;


    if ( s[pos] != '}' ) {
        return false;
    }
    pos ++;   

    id = findPosInVectors(  vp,  make_pair(num1, num2 ) );
    return true;
}

template <class T> inline string to_string (const T& t)
{
    stringstream ss;
    ss << t;
    return ss.str();
}

void fixbrokendictionary (string& s) {
    long cb;
    long sb;
    long ob;
    bool is_string;
    bool is_outer_string = false;
    long j;
    bool colon;
    bool comma;
    bool onlyWS;
    for (long i = 0; i < s.length() ; i++ ) {
        if ( s[i] == '\"' && s[i-1] != '\\') {
            is_outer_string = !is_outer_string;
            continue;
        }
        if (s[i] == '{' && !is_outer_string) {
            cb = 1, sb = 0, ob = 0, is_string = false;
            j = i + 1;
            colon = false;
            comma = false;
            onlyWS = true;
            for (; !(cb == 0 && sb == 0 && ob == 0 && !is_string ) ; j++ ) {
                if (s[j] != ' ' && s[j] != '\n' && s[j] != '}' )
                    onlyWS = false;
                if (s[j] == '\"' && s[j-1] != '\\' ) 
                    is_string = !is_string;
                else if (s[j] == '{' && !is_string)
                    cb++;
                else if (s[j] == '}' && !is_string)
                    cb--;
                else if (s[j] == '[' && !is_string)
                    sb++;
                else if (s[j] == ']' && !is_string)
                    sb--;
                else if (s[j] == '(' && !is_string)
                    ob++;
                else if (s[j] == ')' && !is_string)
                    ob--;
                else if (cb == 1 && sb == 0 && ob == 0 && !is_string && s[j] == ':')
                    colon = true;
                else if (cb == 1 && sb == 0 && ob == 0 && !is_string && s[j] == ',')
                    comma = true;
                
            }
            if ( !colon && !comma) { // colon and comma are used to check for empty dictionary or empty string?
                if (!onlyWS) {
                    s[i] = ' ';
                    s[j-1] = ' ';
                }
                else {
                    s[i] = '\"';
                    s[j-1] = '\"';                    
                }
            }
            continue;
        }
    }
    return;        
}



string trimWS (string s) {
    string rv;
    long i, j;
    for (i = 0; ( s[i] == ' ' || s[i] == '\n' || s[i] == '\r' ) && (i < s.length()) ; i++);
    for (j = s.length() - 1; (s[j] == ' ' || s[j] == '\n' || s[i] == '\r')  && (j >= 0 ) ; j--) ;
    if (i == s.length() || j == -1)
        return "";
    for (; i <= j ; i++) {
        rv.append(1, s[i]);
    }
    return rv;
}