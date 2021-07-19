#include<iostream>
#include<fstream>
#include <vector>
#include <string>
#include <sstream> 
#include <cstdlib>
#include <map> 
#include <set>
#include <iterator> 
#include <utility>
#include <queue>
#include <ctype.h>
#include "../macros.hpp"
#include "../utility.hpp"
#include "AGpropertyresolve.hpp"

using namespace std;

string resolveAGproperty ( string P) {
    string word;
    string argstring;
    vector<string> args; 

    word = readwordfromstring(P);

    argstring = readstringbetween(  P, '(', ')'); // arrays and dict in between!
    
    args = splitavoidarrdictfun(argstring, ","); 

    for (long i = 0; i < args.size(); i++) {
        args[i] = trimWS( args[i] );
    }

    if ( word == "and") 
        return andfn(args); 
    else if ( word == "or") 
        return orfn(args);
    else if ( word == "not") 
        return notfn(args);
    else if ( word == "iskeydefined")
        return iskeydef(args);
    else {
        cerr << "Function " << word << " is not compatible with spec!\n";
        abort();
    }

    return "";
}

string andfn (vector<string> args) {
    vector<string> argsval;
    if ( args.size() <= 1 ) {
        cerr << args.size() << " arguments to 'and' function instead of >=2." << endl;
        abort(); 
    }
    for (long i = 0; i < args.size(); i++) {
        if (args[i] != "T" && args[i] != "F") ;
          //  argsval.push_back( checkproperty(args[i]) ); // Need to define checkproeprty! 
        else ;
            // argsval.push_back( args[i] ) ; // Dunno cause of error!    
    }
    for (long i = 0; i < argsval.size(); i++ ) {
        if (argsval[i] == "F") {
            return "F";
        }    
    }
    return "T";
}

string orfn (vector<string> args) {
    vector<bool> argsval;
    if ( args.size() <= 1 ) {
        cerr << args.size() << " arguments to 'or' function instead of >=2." << endl;
        abort(); 
    }
    for (long i = 0; i < args.size(); i++) {
        if (args[i] != "T" && args[i] != "F") ;
        //    argsval.push_back( checkproperty(args[i]) );  
        else ;
          //  argsval.push_back ( args[i] ) ;      
    }
    for (long i = 0; i < argsval.size(); i++ ) {
        bool temp = argsval[i];
        if ( temp == true ) {
            return "T";
        }    
    }
    return "F";
}

string notfn (vector<string> args) {
    if ( args.size() != 1 ) {
        cerr << args.size() << " arguments to 'not' function instead of 1." << endl;
        abort(); 
    }
    // if (checkproperty( args[0] ) == "F")
    //     return "T";   
    // else
    //     return "F";

    return "";
}


// Need to implement readDictfromstring!
string iskeydef (vector<string> args) {
    map<string, string> d;
    string key;
    if ( args.size() != 2 ) {
        cerr << args.size() << " arguments to 'iskeydefined' function instead of 2." << endl;
        abort(); 
    }
    key = trimWS(args[1]);
    key.erase(0, 1);
    key.erase(key.length()-1, 1);

    d = readDictfromstring ( args[0] );
    if (d.find(key) != d.end() )
        return "T";
    else
        return "F";
    
    return "";
}

string dictval (vector<string> args) {
    map<string, string> d;
    string key;
    if ( args.size() != 2 ) {
        cerr << args.size() << " arguments to 'iskeydefined' function instead of 2." << endl;
        abort(); 
    }
    key = trimWS(args[1]);
    d = readDictfromstring ( args[0] );

    if (d.find(key) == d.end() )
        return "";
    else
        return d[key];
    
    return "";
}