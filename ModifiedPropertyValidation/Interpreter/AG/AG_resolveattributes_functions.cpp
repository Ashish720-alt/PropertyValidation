#include<iostream>
#include<set>
#include<vector>
#include<map>
#include<string>
#include<fstream>
#include<stdlib.h>
#include "AG_resolveattributes_functions.hpp" 
#include "AGmacros.hpp"
#include "AGutility.hpp"
#include "../macros.hpp"
#include "../utility.hpp"
#include "../printFunctions.hpp"
#include "AG_resolveattributes.hpp"

using namespace std;

string AG_make_singleton_array(vector<string> arguments) {
    if ( arguments.size() != 1 ) 
        abort_with_error_message( arguments.size() + " arguments to make_singleton_array function instead of 1.\n" );      
    string argument = arguments[0];
    string retval = (argument == "") ? "[ ]" : ("[ " + argument + " ]") ;
    return retval;
}


string AG_appendallChildren(vector<string> arguments) {
    if ( arguments.size() != 2 ) 
        abort_with_error_message( arguments.size() + " arguments to append_all_children_attributes function instead of 2.\n" );      
    long ID = stringToInt( arguments[0]);
    string key = readstringbetween( arguments[1], '<', '>' );
    
    if (adjacencylist.find(ID) == adjacencylist.end())
        abort_with_error_message("ID argument to append_all_children_attributes not found!\n");
    vector<long> kids = adjacencylist[ID];
    vector<string> retval = vector<string> ();
    for (long i = 0; i < kids.size(); i++) {
        map<string, string> dict = dictionaries[  kids[i] ];
        vector<string> temp_add = (dict.find(key) != dict.end() ) ? readArrayfromstring( dict[key] ) : vector<string> ();
        
        retval.insert( retval.end(),  temp_add.begin(), temp_add.end()  );
    }

    return vector_of_strings_to_string(retval) ;

}

string AG_union(vector<string> arguments) {
    if ( arguments.size() <= 1 ) 
        abort_with_error_message( arguments.size() + " arguments to append function instead of > 1.\n" );     
    for (long i = 0; i < arguments.size(); i++) {
        if( !is_enclosed_by_delimiters( arguments[i], '[', ']'  ) )
            arguments[i] = solve_attribute_value( arguments[i]) ;
    }
    vector<string> final_string ;
    for (long i = 0; i < arguments.size(); i++) {
        vector<string> temp_string = readArrayfromstring( arguments[i] );
        final_string.insert( final_string.end(),  temp_string.begin(), temp_string.end()  );
    }

    // Remove empty strings!
    for (long i = 0; i < final_string.size(); ) {
        string temp = final_string[i];
        if (temp == "") {
            final_string.erase( final_string.begin() + i );
            continue;
        }
        i++;
    }
    return vector_of_strings_to_string(final_string) ;
}



string AG_dictval(vector<string> arguments) {
    if ( arguments.size() != 2 ) 
        abort_with_error_message( arguments.size() + " arguments to dictval function instead of 2.\n" );     
    map<string, string> dict = readDictfromstring( arguments[0] );
    string key = arguments[1];
    return dict[key] ;
}


string string_andfn (vector<string> args) {
    bool retval = true;
    if ( args.size() <= 1 ) 
        abort_with_error_message(args.size() + " arguments to 'and' function instead of >=2.\n");
    for (long i = 0; i < args.size(); i++) {
        if (args[i] != str(True) && args[i] != str(False))
            args[i] = solve_attribute_value(args[i]);
        if ( args[i] == str(True) ) 
            retval &= true;
        else if ( args[i] == str(False) ) 
            retval &= false;
        else
            abort_with_error_message("Unexpected string argument to 'and' function.");   
    }
    return (retval ? str(True) : str(False) );
}

string string_orfn (vector<string> args) {
    bool retval = false;
    if ( args.size() <= 1 ) 
    abort_with_error_message(args.size() + " arguments to 'or' function instead of >=2.\n");
    for (long i = 0; i < args.size(); i++) {
        if (args[i] != str(True) && args[i] != str(False))
            args[i] = solve_attribute_value(args[i]);
        if ( args[i] == str(True) ) 
            retval |= true;
        else if ( args[i] == str(False) ) 
            retval |= false;
        else
            abort_with_error_message("Unexpected string argument to 'or' function.");   
    }
    return (retval ? str(True) : str(False) );
}

string string_notfn (vector<string> args) {
    bool retval = false;
    if ( args.size() != 1 ) 
        abort_with_error_message(args.size() + " arguments to 'not' function instead of 1.\n");
    if (args[0] != str(True) && args[0] != str(False))
        args[0] = solve_attribute_value(args[0]);
    if ( args[0] == str(True) ) 
        retval = false;
    else if ( args[0] == str(False) ) 
        retval |= true;
    else
        abort_with_error_message("Unexpected string argument to 'or' function.");   

    return (retval ? str(True) : str(False) );
}


string iskeydef (vector<string> args) {
    if ( args.size() != 2 ) 
        abort_with_error_message(args.size() + " arguments to 'iskeydefined' function instead of 2.\n");
    long node_id = stringToInt( args[0] );
    string key_value = readstringbetween( args[1], '<', '>' );
    map<string, string> dict = dictionaries[node_id];

    return ( (dict.find(key_value) == dict.end() )  ? str(False) : str(True) );
}

