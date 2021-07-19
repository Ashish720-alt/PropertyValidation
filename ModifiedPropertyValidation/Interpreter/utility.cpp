#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream> 
#include <utility>
#include <cstddef>  
#include <cstdlib>
#include <ctype.h>
#include <map>
#include <set>
#include <iterator> 
#include <utility>
#include <stdlib.h>
#include "utility.hpp"

using namespace std;

extern map<long,  map <string, string> > dictionaries;
extern vector< long > Sources;
extern vector< long > Leaves;
extern vector<long >  Graph; 
extern map<long, vector<long> > adjacencylist;
extern bool cycle ;
extern bool connected ;
extern bool orderedtree;
extern string graph_type;

vector<string> split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

pair<string,string> singleSplit (string s, string delimiter) {
    pair<string,string> temp;
    size_t index = s.find( delimiter );
    if (index == string::npos) {
        cerr << delimiter << " does not occur in the \"" << s << "\"" << endl;
        abort();
    }
    temp.first =  s.substr(0, index);
    temp.second = s.substr(index + delimiter.size());
    return temp;
}

vector<string> splitavoidarrdictfun (string s, string delimiter) {
    if (s.empty())
        return vector<string> () ;
    
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;
    vector<bool> inarrdictfun;
    long cb;
    long sb;
    long ob;
    bool instring = false;

    cb = 0;
    sb = 0;
    ob = 0;
    for (long i = 0; i < s.length(); i++) {
        if (s[i] == '{')
            cb++;
        else if (s[i] == '}')
            cb--;
        else if (s[i] == '[')
            sb++;
        else if (s[i] == ']')
            sb--;  
        else if (s[i] == '(')
            ob++;
        else if (s[i] == ')')
            ob--;        
        else if (s[i] == '\"')
            instring = !instring;

        if (cb == 0 && sb == 0 && ob == 0 && !instring)
            inarrdictfun.push_back(false);
        else    
            inarrdictfun.push_back(true);        
    }

    size_t pos_start2 = pos_start;
    while ((pos_end = s.find (delimiter, pos_start2)) != string::npos) {
        if (!inarrdictfun[pos_end] ) {
            token = s.substr (pos_start, pos_end - pos_start);
            res.push_back (token);
            pos_start = pos_end + delim_len;
            pos_start2 = pos_start;
            continue;
        }
        pos_start2 = pos_end + delim_len;
    }

    res.push_back (s.substr (pos_start));
    return res;
}

long stringToInt (string s) {
    long num = 0;
    long i;
    bool negative = false;
    bool singledig = false;
    for (i = 0; (  isWS( s[i]) ) && (i < s.length()) ; i++ );
    if (i == s.length()) {
        cerr << "Is an empty string, not a number\n";
        abort();
    }
    if (!isdigit(s[i]) && s[i] != '-') {
        cerr << "Input string is not a number, has charactar " << s[i] << endl;
        abort();
    }
    if (s[i] == '-') {
        negative = true;
    }
    else {
        num = (num * 10) + (s[i] - '0');
    }
    i++;
    for (; (i < s.length()) && !(isWS( s[i]) ); i ++) {
        if (!singledig && negative)
            singledig = true;
        if(! (isdigit(s[i]) ) ) {
            cerr << "Input string is not a number, has inbetween charactar " << s[i] << endl;
            abort();   
        }
        num = (num * 10) + (s[i] - '0');
    }
    if (!singledig && negative) {
        cerr << "Input string " << s << " doesn't contain a single digit." << endl;
        abort(); 
    }
    if (i != s.length() ) {
        for ( ; i < s.length(); i++) {
            if (!isWS( s[i])) {
                cerr << "Input string is not a number, has charactar " << s[i] << endl;
                abort();
            }
        }
    }
    if (negative)
        num = 0 - num;
    return num;
}

bool isStringNum (string s) {
    long i;
    bool negative = false;
    bool singledig = false;
    for (i = 0; (isWS( s[i]) ) && (i < s.length()) ; i++ );
    if (i == s.length()) 
        return false;
    if (!isdigit(s[i]) && s[i] != '-') 
        return false;
    if (s[i] == '-') 
        negative = true;
    i++;
    for (; (i < s.length()) && !(isWS( s[i])); i ++) {
        if (!singledig && negative)
            singledig = true;
        if(! (isdigit(s[i]) ) ) 
            return false;
    }
    if (!singledig && negative) 
        return false;
    if (i != s.length() ) {
        for ( ; i < s.length(); i++) 
            if (isWS( s[i])) 
                return false;
    }
    return true;
}

char charEOFabort (fstream& file) {
    char newchar;
    file.get(newchar);
    if (file.eof() ) {
        cerr << "Unexpected file format, input file ends here!\n";
        abort();
    }   
    return  newchar ;
}


string peekwordignoreWSEOFabort (fstream& file) {
    char peek;
    string word;
    char c;
    peek = file.peek();
    c = (char ) peek;
    if (peek == EOF) {
        cerr << "Unexpected file format, input file ends here!\n";
        abort();        
    }
    while ( isWS( c) ) {
        file.get( c );
        peek = file.peek();
        if (peek == EOF) {
            cerr << "Unexpected file format, input file ends here!\n";
            abort();        
        }
        c = (char) peek;
    }
    
    std::istream::streampos p = file.tellg();

    while (!isWS(c) ) {
        file.get(c);
        word.push_back(c);
        c = (char) file.peek();
    }

    file.seekg(p);

    return word ;
}    


char peekcharignoreWSEOFabort (fstream& file) {
    int peek;
    char c;
    peek = file.peek();
    c = (char ) peek;
    if (peek == EOF) {
        cerr << "Unexpected file format, input file ends here!\n";
        abort();        
    }
    while ( isWS( c) ) {
        file.get( c );
        peek = file.peek();
        if (peek == EOF) {
            cerr << "Unexpected file format, input file ends here!\n";
            abort();        
        }
        c = (char) peek;
    }
    return c ;
}

char getcharignoreWSEOFabort (fstream& file) {
    char newchar ;
    file.get(newchar);
    if ( file.eof() ) {
        cerr << "Unexpected file format, input file ends here!\n";
        abort();
    }     
    while (isWS( newchar )) {
        file.get( newchar );
        if (file.eof() ) {
            cerr << "Unexpected file format, input file ends here!\n";
            abort();
        } 
    }  
    return  newchar;
}

char charcompareabort (fstream& file, const char c1) {
    if (isWS( c1)) {
        cerr << "Input charactar is the WS charactar: " << c1 << endl;
        abort();
    }
    char newchar = getcharignoreWSEOFabort(file);
    if (newchar != c1) {
        cerr << "Unexpected file format, expected "<< c1 <<" here!\n";
        abort();
    }   
    return newchar;
}

string stringcompareabort (fstream& file, const string s) {
    if ( s.empty() ) {
        return s; 
    }
    charcompareabort( file, s[0]);
    for (long i = 1; i < s.length(); i++) {
        char c = charEOFabort( file );
        if (c != s[i]) {
            cerr << "Unexpected file format, expected "<< s[i] <<" here!\n";
            abort();
        }   
    }
    return s;
}

char chardigabort (fstream& file) {
    char newchar = getcharignoreWSEOFabort(file);
    if ( !isdigit(newchar) && newchar != '-' ) {
        cerr << "Unexpected file format, expected number here!\n";
        abort();
    }    
    return newchar;
}

string readword (fstream& file ) {
    char c = getcharignoreWSEOFabort( file );
    string rv;
    if (!isalpha(c) ){
        cerr << "First non-WS charactar is not an alphabet charactar, it is: " << c << endl;
        abort();
    }
    while ( isalpha(c) || isdigit(c) || c == '_') {
        rv.append(1, c);
        c = charEOFabort(file);
    }
    return rv;
}

string readstring (fstream& file) {
    char c = charcompareabort( file, '\"');
    string text(1, c);
    c = 'a';
    char prev = '\"' ;
    while ( !(c == '\"' && prev != '\\' )  ) {
        prev = c;
        c = charEOFabort(file);
        text.append(1 ,c);
    }
    return text;
}

string readValueArray (fstream& file) {
    char c = charcompareabort( file, '[');
    long cb = 0;
    long sb = 1;
    string value(1, c);
    c = 'a';
    bool instring = false;
    while ( c != ']' || cb != 0 || sb != 0) {
        c = charEOFabort(file);
        if (c == '[' && !instring )
            sb++;
        else if (c == ']' && !instring )
            sb--;
        else if (c == '{' && !instring )
            cb++;
        else if (c == '}' && !instring )
            cb--;
        value.append(1 ,c);
    }
    return value;
}

string readValueDict (fstream& file) {
    char c = charcompareabort( file, '{');
    long cb = 1;
    long sb = 0;
    string value(1, c);
    c = 'a';
    bool instring = false;
    while ( c != '}' || cb != 0 || sb != 0) {
        c = charEOFabort(file);
        if (c == '\"')
            instring = !instring;
        else if (c == '[' && !instring)
            sb++;
        else if (c == ']' && !instring )
            sb--;
        else if (c == '{' && !instring )
            cb++;
        else if (c == '}' && !instring )
            cb--;
        value.append(1 ,c);
    }
    return value;
}

string readValueInt (fstream& file) {
    bool neg = false;
    char c = chardigabort(file);
    string value;
    if (c == '-') {
        neg = true;
        value.append(1, c);
        c = '0';
    }
    while ( isdigit(c) ) {
        if (!neg) {
            value.append(1 ,c);
        }
        else {
            neg = false;
        }
        c = charEOFabort(file);
    }
    if (value.size() == 1 && value[0] == '-') {
        cerr << "Not a valid number, only contains - sign\n";
        abort();
    if ( c != ' ' || c != '\n') {
        cerr << "Not a valid number, final charactar is: " << c << endl;
        abort();
    }
    }
    return value;
}

string readwordfromstring (string s ) {
    char c;
    bool onlyWS;
    string rv;
    long i;

    onlyWS = true;
    for( i = 0 ; i < s.length(); i++) {
        if (!isWS( s[i])) {
            onlyWS = false;
            break;
        }
    }
    if (onlyWS) {
        cerr << "String has only WS charactars!\n";
        abort();
    }

    c = s[i];
    if (!isalpha(c)) {
        cerr << "First non-WS charactar is not a alphabet charactar, it is: " << c << endl;
        abort();
    }

    while ( isalpha(c) || isdigit(c) || c == '_' ) {
        rv.append(1, c);
        i++;
        if ( i >= s.length() )
            break;
        c = s[i];
    }
    return rv;
}

string readstringbetween (string s, const char c1, const char c2) {
    long i;
    string rv;
    long cb, sb, ob;
    for (i = 0; i < s.length(); i ++) {
        if ( s[i] == c1 )
            break; 
    }
    if (i == s.length()) {
        cerr << s << " has no occurrence of " << c1;
        abort();
    }
    
    i++;
    cb = 0;
    sb = 0;
    ob = 0;
    for( ; (s[i] != c2) || (cb != 0 || sb != 0 || ob != 0) ; i++ ) {
        if (i == s.length() - 1) {
            cerr << "String has " << c1 << " but no occurrence of " << c2 << endl;
            abort();
        }
        if (s[i] == '[')
            sb ++;
        else if (s[i] == ']')
            sb--;
        else if (s[i] == '{')
            cb++;
        else if (s[i] == '}')
            cb--;
        else if (s[i] == '(')
            ob++;
        else if (s[i] == ')')
            ob--;
        rv.append(1, s[i]);
    }
    return rv;
    
}

string trimWS (string s) {
    string rv;
    long i, j;
    for (i = 0; (isWS( s[i])) && (i < s.length()) ; i++);
    for (j = s.length() - 1; (s[j] == ' ' || s[j] == '\n') && (j >= 0 ) ; j--) ;
    if (i == s.length() || j == -1)
        return "";
    for (; i <= j ; i++) {
        rv.append(1, s[i]);
    }
    return rv;
}

bool isWS (char c) {
    return (c == ' ' || c == '\n' || c == '\t' || c == '\r') ;
}

char peekcharignoreWSEOFreturn (fstream& file, bool& eof) {
    int peek;
    char c = 0;
    eof = false;

    peek = file.peek();
    c = (char ) peek;
    if (peek == EOF) {
        eof = true;
        return c; 
    }
    while ( isWS( c) ) {
        file.get( c );
        peek = file.peek();
        if (peek == EOF) {
            eof = true;
            return c;
        }
        c = (char) peek;
    }
    return c ;
}

bool is_enclosed_by_delimiters (string s, char start_delimiter, char end_delimiter) {
    return (s[0] == start_delimiter && s[s.length() - 1]  == end_delimiter) ;
}

void abort_with_error_message ( string s) {
    cerr << s;
    abort();
}


string readstringbetween_abort_iff_c1_and_no_c2 (string s, const char c1, const char c2) {
    long i;
    string rv;
    long cb, sb, ob;
    for (i = 0; i < s.length(); i ++) {
        if ( s[i] == c1 )
            break; 
    }
    if (i == s.length()) {
        return "";
    }
    
    i++;
    cb = 0;
    sb = 0;
    ob = 0;
    for( ; (s[i] != c2) || (cb != 0 || sb != 0 || ob != 0) ; i++ ) {
        if (i == s.length() - 1) {
            cerr << "String has " << c1 << " but no occurrence of " << c2 << endl;
            abort();
        }
        if (s[i] == '[')
            sb ++;
        else if (s[i] == ']')
            sb--;
        else if (s[i] == '{')
            cb++;
        else if (s[i] == '}')
            cb--;
        else if (s[i] == '(')
            ob++;
        else if (s[i] == ')')
            ob--;
        rv.append(1, s[i]);
    }
    return rv;
    
}

pair<string, string> trimWS_pair ( pair<string, string> ps) {
    pair<string, string> retval;
    retval.first = trimWS( ps.first );
    retval.second = trimWS( ps.second );
    return retval;
}

vector<string> trimWS_vector (vector<string> vs) {
    vector<string> retval;
    for (long i = 0 ; i< vs.size(); i++) 
        retval.push_back( trimWS( vs[i] )) ;
    return retval;
}


string vector_of_Numbers_to_string (vector <long > v ) {
    string rv;
    rv.append("[ ");
    for (long i = 0; i < v.size(); i++) {
         if (i != v.size() - 1)
             rv.append( to_string(v[i]) + ", " ) ;
         else
             rv.append( to_string(v[i]) + " " ) ;
    }
    rv.append("]");
    return rv;
}

string vector_of_strings_to_string (vector <string > v ) {
    string rv;
    rv.append("[ ");
    for (long i = 0; i < v.size(); i++) {
         if (i != v.size() - 1)
             rv.append( v[i] + ", " ) ;
         else
             rv.append( v[i] + " " ) ;
    }
    rv.append("]");
    return rv;
}


map<string, string> readDictfromstring (string s) {
    string modified_str = readstringbetween(s, '{', '}');
    vector<string> key_val_pairs = trimWS_vector( splitavoidarrdictfun( modified_str, "," ) ); 
    map<string, string> retval;
    for (long i = 0; i < key_val_pairs.size(); i++) {
        pair<string, string> key_val_pair = singleSplit( key_val_pairs[i], ":" );
        retval.insert( key_val_pair );
    }

    return retval;
}

vector<string> readArrayfromstring (string s) {
    if (s.empty())
        return vector<string> ();
    string modified_str = readstringbetween(s, '[', ']');
    vector<string> retval = trimWS_vector( splitavoidarrdictfun( modified_str, "," ) );
    return retval;     
}