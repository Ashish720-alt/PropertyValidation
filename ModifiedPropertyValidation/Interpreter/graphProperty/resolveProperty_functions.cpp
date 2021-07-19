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
#include "resolveProperty.hpp"
#include "resolveProperty_functions.hpp"
#include "graphmacros.hpp"
#include "../printFunctions.hpp"

using namespace std;


bool graph_l(vector<string> arguments) {
    if ( arguments.size() != 2 ) 
        abort_with_error_message( arguments.size() + " arguments to constant value < instead of 2.\n" );      
    long num1 = stringToInt( arguments[0] ) ;
    long num2 = stringToInt( arguments[1] ) ;
    return (num1 < num2);
}

bool graph_le(vector<string> arguments) {
    if ( arguments.size() != 2 ) 
        abort_with_error_message( arguments.size() + " arguments to constant value <= instead of 2.\n" );      
    long num1 = stringToInt( arguments[0] ) ;
    long num2 = stringToInt( arguments[1] ) ;
    return (num1 <= num2);
}

bool graph_g(vector<string> arguments) {
    if ( arguments.size() != 2 ) 
        abort_with_error_message( arguments.size() + " arguments to constant value > instead of 2.\n" );      
    long num1 = stringToInt( arguments[0] ) ;
    long num2 = stringToInt( arguments[1] ) ;
    return (num1 > num2);
}

bool graph_ge(vector<string> arguments) {
    if ( arguments.size() != 2 ) 
        abort_with_error_message( arguments.size() + " arguments to constant value >= instead of 2.\n" );      
    long num1 = stringToInt( arguments[0] ) ;
    long num2 = stringToInt( arguments[1] ) ;
    return (num1 >= num2);
}


bool graph_trueConstant (vector<string> arguments) {
    if ( arguments.size() != 0 ) 
        abort_with_error_message( arguments.size() + " arguments to constant value \"True\" instead of 0.\n" );    

    return true;
}

bool graph_falseConstant (vector<string> arguments) {
    if ( arguments.size() != 0 ) 
        abort_with_error_message( arguments.size() + " arguments to constant value \"False\" instead of 0.\n" );    

    return false;
}

bool graph_andfn (vector<string> arguments) {
    if ( arguments.size() <= 1 ) 
        abort_with_error_message( arguments.size() + " arguments to 'and' function instead of >=2.\n" );

    bool does_property_hold = true;
    for (long i = 0; i < arguments.size(); i++) 
        does_property_hold &= graph_checkproperty(arguments[i]);    

    return does_property_hold;
}

bool graph_implies (vector<string> arguments) {
    if ( arguments.size() != 2 ) 
        abort_with_error_message( arguments.size() + " arguments to 'and' function instead of 2.\n" );

    bool does_property_hold_first;
    bool does_property_hold_second;
    bool rv;

    does_property_hold_first = graph_checkproperty(arguments[0]);
    does_property_hold_second = graph_checkproperty(arguments[1]);

    rv = (!does_property_hold_first) | does_property_hold_second;

    return rv;
}

bool graph_orfn (vector<string> arguments) {
    if ( arguments.size() <= 1 ) 
            abort_with_error_message( arguments.size() + " arguments to 'or' function instead of >=2.\n" );

    bool does_property_hold = false;
    for (long i = 0; i < arguments.size(); i++)
            does_property_hold |= graph_checkproperty(arguments[i]);     

    return does_property_hold;
}

bool graph_notfn (vector<string> arguments) {
    if ( arguments.size() != 1 ) 
        abort_with_error_message( arguments.size() + " arguments to 'not' function instead of 1.\n" );
    
    return !(graph_checkproperty( arguments[0] ) );   
}

bool graph_innfn (vector<string> arguments) {
    if ( arguments.size() != 2 ) 
        abort_with_error_message(arguments.size() + " arguments to 'inn' function instead of 2.\n" );
    
    long num = stringToInt( arguments[0] ) ;
    string array_as_string = trimWS(readstringbetween( arguments[1], '[', ']'));
    if (array_as_string.empty())
        return false;
    
    vector<string> array_numbers =  split( array_as_string , "," );
    long array_number;
    for (long i = 0; i < array_numbers.size(); i++) {
        array_number = stringToInt(   trimWS(array_numbers[i]) );
        if (num == array_number)
            return true;
    }
    return false;
}

bool graph_insfn (vector<string> arguments) {
    if ( arguments.size() != 2 ) 
        abort_with_error_message(arguments.size() + " arguments to 'ins' function instead of 2.\n" );

    string str = arguments[0] ;
    string array_as_string = trimWS(readstringbetween( arguments[1], '[', ']'));
    if (array_as_string.empty())
        return false;

    vector<string> array_strings =  split( array_as_string , "," );
    string array_string;
    for (long i = 0; i < array_strings.size(); i++) {
        array_string = trimWS(array_strings[i]) ;
        if (str.compare(array_string) == 0)
            return true ;
    }
    return false;    
}

bool graph_eqnfn (vector<string> arguments) {
    if ( arguments.size() != 2 ) 
        abort_with_error_message(arguments.size() + " arguments to 'eqn' function instead of 2.\n" );

    long first_number = stringToInt( arguments[0] ) ;
    long second_number = stringToInt( arguments[1] ) ;
    return (first_number == second_number);
}

bool graph_eqsfn (vector<string> arguments) {
    if ( arguments.size() != 2 ) 
        abort_with_error_message(arguments.size() + " arguments to 'eqs' function instead of 2.\n" );

    string str_first = arguments[0] ;
    string str_second = arguments[1] ;
    return (str_first.compare(str_second) == 0) ;
}


