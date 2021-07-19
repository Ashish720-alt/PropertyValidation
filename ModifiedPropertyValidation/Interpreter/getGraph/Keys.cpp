#include<iostream>
#include<cstdlib>
#include<set>
#include<string>
#include<fstream>
#include "Keys.hpp"

using namespace std;

set<string> keylist (fstream& f) {
    char c;
    set<string> rv;
    string temp;

    f.clear();                 
    f.seekg(0, ios::beg); 

    while( !f.eof() ) {
        c = f.get();
        if (c == '<') {
            temp.clear();
            c = f.get();
            while ( c != '>') {
                temp.append(1, c);
                c = f.get();
            }
            if (temp[0] != '\"' || temp[ (temp.size() - 1)] != '\"' )  // Is an attribute reference!
                continue;      
            rv.insert( temp );
        }
    }  
    return rv;
}

