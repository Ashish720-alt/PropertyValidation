#ifndef UTILITY_HPP
#define UTILITY_HPP

#include<fstream>
#include <vector>
#include <string>
#include <set>

using namespace std;


vector<string> split (string s, string delimiter);


pair<string,string> singleSplit (string s, string delimiter) ;


vector<string> splitavoidarrdictfun (string s, string delimiter);


long stringToInt (string s); 


bool isStringNum (string s);


template <typename T> long getVectorIndex (vector <T > v, T node ) {
    for (long i = 0; i < v.size(); i++) {
        if (v[i] == node)
            return i;
    }
    return -1;
}

template <typename T> bool is_element_in_Vector (vector <T > v, T node ) {
    return (getVectorIndex(v, node) != -1 ) ; 
}


char charEOFabort (fstream& file);


char peekcharignoreWSEOFabort (fstream& file) ;


char getcharignoreWSEOFabort (fstream& file);


char charcompareabort (fstream& file, const char c1);


string stringcompareabort (fstream& file, const string s);


char chardigabort (fstream& file);


string readword (fstream& file ) ;


string readstring (fstream& file) ;

string readValueArray (fstream& file) ;

string readValueDict (fstream& file);

string readValueInt (fstream& file);

string readwordfromstring (string s );

string readstringbetween (string s, const char c1, const char c2) ;

string trimWS (string s);

bool isWS (char c);

char peekcharignoreWSEOFreturn (fstream& file, bool& eof) ;

string convertdicttostring ( map<string, string> d) ;

map<string, string> readDictfromstring (string s) ;

vector<string> readArrayfromstring (string s) ;

bool is_enclosed_by_delimiters (string s, char start_delimiter, char end_delimiter) ;

void abort_with_error_message ( string s);

string readstringbetween_abort_iff_c1_and_no_c2 (string s, const char c1, const char c2) ;

pair<string, string> trimWS_pair ( pair<string, string> ps);
vector<string> trimWS_vector (vector<string> vs);

template <typename T, typename S> vector<T> get_keys_of_map_as_vector ( map<T,S> d ) {
    vector<T> retval;
    for (typename map<T,S>::iterator it = d.begin() ; it!= d.end() ; ++it) {
        retval.push_back( it->first );
    }
    return retval;
}

string vector_of_Numbers_to_string (vector <long > v ) ;

string vector_of_strings_to_string (vector <string > v ) ;


#endif


 