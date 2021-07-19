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
#include "GraphPropertySpec.hpp"
#include "IRGraphTraversal.hpp"

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


bool checkproperty ( string P) {
    string word;
    string argstring;
    vector<string> args; 

    word = readwordfromstring(P);

    if (word == "True")
        return true;
    else if ( word == "False")
        return false;

    argstring = readstringbetween(  P, '(', ')'); // arrays and dict in between!
    
    args = splitavoidarrdictfun(argstring, ","); 

    for (long i = 0; i < args.size(); i++) {
        args[i] = trimWS( args[i] );
    }

    if ( word == "and") {
        return andfn(args); 
    }
    else if ( word == "or") 
        return orfn(args);
    else if ( word == "not") 
        return notfn(args);
    else if ( word == "inn") 
        return innfn(args);
    else if ( word == "ins")
        return insfn(args);
    else if ( word == "eqn")
        return eqnfn(args); 
    else if ( word == "eqs")
        return eqsfn(args);
    else {
        cerr << "Function " << word << " is not compatible with spec!\n";
        abort();
    }

    return false;
}

bool andfn (vector<string> args) {
    vector<bool> argsval;
    if ( args.size() <= 1 ) {
        cerr << args.size() << " arguments to 'and' function instead of >=2." << endl;
        abort(); 
    }
    for (long i = 0; i < args.size(); i++) {
        if (args[i] == "True")
            argsval.push_back ( true );
        else if (args[i] == "False")
            argsval.push_back ( false );
        else 
            argsval.push_back( checkproperty(args[i]) );      
    }
    for (long i = 0; i < argsval.size(); i++ ) {
        if (argsval[i] == false) {
            return false;
        }    
    }
    return true;
}

bool orfn (vector<string> args) {
            vector<bool> argsval;
        if ( args.size() <= 1 ) {
            cerr << args.size() << " arguments to 'or' function instead of >=2." << endl;
            abort(); 
        }
    for (long i = 0; i < args.size(); i++) {
        if (args[i] == "True")
            argsval.push_back ( true );
        else if (args[i] == "False")
            argsval.push_back ( false );
        else 
            argsval.push_back( checkproperty(args[i]) );      
    }
    for (long i = 0; i < argsval.size(); i++ ) {
        if (argsval[i] == true) {
            return true;
        }    
    }
    return false;
}

bool notfn (vector<string> args) {
    if ( args.size() != 1 ) {
        cerr << args.size() << " arguments to 'not' function instead of 1." << endl;
        abort(); 
    }
    return !(checkproperty( args[0] ) );   
}

bool innfn (vector<string> args) {
    if ( args.size() != 2 ) {
        cerr << args.size() << " arguments to 'inn' function instead of 2." << endl;
        abort(); 
    }
    long num = stringToInt( trimWS(args[0]) ) ;
    vector<long> arr_num;
    string s = readstringbetween( args[1], '[', ']');
    if (trimWS(s) == "")
        return false;
    vector<string> num_string =  split( s , "," );
    for (long i = 0; i < num_string.size(); i++)
        arr_num.push_back( stringToInt(   trimWS(num_string[i]) ) );
    for (long i = 0; i < arr_num.size(); i++) {
        if (arr_num[i] == num)
            return true;
    }
    return false;
}

bool insfn (vector<string> args) {
    if ( args.size() != 2 ) {
        cerr << args.size() << " arguments to 'ins' function instead of 2." << endl;
        abort(); 
    }
    string str = trimWS(args[0]) ;
    string s = readstringbetween( args[1], '[', ']');
    if (trimWS(s) == "")
        return false;
    vector<string > arr_str = split( s , "," );
    for (long i = 0; i < arr_str.size(); i++) {
        arr_str[i] = trimWS( arr_str[i] ); 
    }
    for (long i = 0; i < arr_str.size(); i++) {
        if (arr_str[i] == str)
            return true;
    }
    return false;    
}

bool eqnfn (vector<string> args) {
    if ( args.size() != 2 ) {
        cerr << args.size() << " arguments to 'eqn' function instead of 2." << endl;
        abort(); 
    }
    long num1 = stringToInt( trimWS(args[0]) ) ;
    long num2 = stringToInt( trimWS(args[1]) ) ;
    return (num1 == num2);
}

bool eqsfn (vector<string> args) {
    if ( args.size() != 2 ) {
        cerr << args.size() << " arguments to 'eqs' function instead of 2." << endl;
        abort(); 
    }
    string str1 = trimWS(args[0]) ;
    string str2 = trimWS(args[1]) ;
    return (str1.compare(str2) == 0) ;
}

long findChildIndex ( string P) {
    string word;
    string argstring;
    vector<string> args; 

    word = readwordfromstring(P);

    argstring = readstringbetween(  P, '(', ')'); // arrays and dict in between!
    
    args = splitavoidarrdictfun(argstring, ","); 

    for (long i = 0; i < args.size(); i++) {
        args[i] = trimWS( args[i] );
    }

    if ( word == "indexn") 
        return indexnfn(args); 
    else if ( word == "indexs") 
        return indexsfn(args);
    else {
        cerr << "Function " << word << " is not compatible with spec!\n";
        abort();
    }

    return -1;
}

long indexnfn (vector<string> args) {
    if ( args.size() != 2 ) {
        cerr << args.size() << " arguments to 'inn' function instead of 2." << endl;
        abort(); 
    }
    long num = stringToInt( trimWS(args[0]) ) ;
    vector<long> arr_num;
    string s = readstringbetween( args[1], '[', ']');
    if (trimWS(s) == "")
        return -1;

    vector<string> num_string =  split( s , "," );
    for (long i = 0; i < num_string.size(); i++)
        arr_num.push_back( stringToInt(   trimWS(num_string[i]) ) );
    for (long i = 0; i < arr_num.size(); i++) {
        if (arr_num[i] == num)
            return (i+1);
    }
    return -1;
}


long indexsfn (vector<string> args) {
    if ( args.size() != 2 ) {
        cerr << args.size() << " arguments to 'ins' function instead of 2." << endl;
        abort(); 
    }
    string str = trimWS(args[0]) ;
    string s = readstringbetween( args[1], '[', ']');
    if (trimWS(s) == "")
        return -1;

    vector<string > arr_str = split( s , "," );
    for (long i = 0; i < arr_str.size(); i++) {
        arr_str[i] = trimWS( arr_str[i] ); 
    }
    for (long i = 0; i < arr_str.size(); i++) {
        if (arr_str[i] == str)
            return (i+1);
    }
    return -1;    
}