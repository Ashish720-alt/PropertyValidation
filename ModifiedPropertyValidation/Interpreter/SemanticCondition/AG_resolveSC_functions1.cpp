#include<iostream>
#include<set>
#include<vector>
#include<map>
#include<string>
#include<fstream>
#include<stdlib.h>
#include "AGmacros1.hpp"
#include "AGutility1.hpp"
#include "../macros.hpp"
#include "../utility.hpp"
#include "../printFunctions.hpp"
#include "AG_resolveSC1.hpp"
#include "AG_resolveSC_functions1.hpp"

using namespace std;



bool AG_issetfn(vector<string> arguments) {
    if ( arguments.size() != 1 ) 
        abort_with_error_message( arguments.size() + " arguments to AG_issetfn function instead of 1.\n" );     
    
    vector<string> arr_string = readArrayfromstring( arguments[0] ) ;
    set<string> set_string;
    set_string.insert(arr_string.begin(), arr_string.end() );

    return (set_string.size() == arr_string.size()) ;
}

bool AG_isemptyarrfun(vector<string> arguments) {
    if ( arguments.size() != 1 ) 
        abort_with_error_message( arguments.size() + " arguments to isempty_a function instead of 1.\n" );     
    string array_contents = trimWS( readstringbetween( arguments[0], '[', ']' ) );
    return (array_contents.empty()) ;
}


bool AG_trueConstant (vector<string> arguments) {
    if ( arguments.size() != 0 ) 
        abort_with_error_message( arguments.size() + " arguments to constant value \"True\" instead of 0.\n" );    

    return true;
}

bool AG_falseConstant (vector<string> arguments) {
    if ( arguments.size() != 0 ) 
        abort_with_error_message( arguments.size() + " arguments to constant value \"False\" instead of 0.\n" );    

    return false;
}

bool AG_andfn (vector<string> arguments) {
    if ( arguments.size() <= 1 ) 
        abort_with_error_message( arguments.size() + " arguments to 'and' function instead of >=2.\n" );

    bool does_property_hold = true;
    for (long i = 0; i < arguments.size(); i++) 
        does_property_hold &= AG_check_0thOrderLogic_property(arguments[i]);    

    return does_property_hold;
}

bool AG_orfn (vector<string> arguments) {
    if ( arguments.size() <= 1 ) 
            abort_with_error_message( arguments.size() + " arguments to 'or' function instead of >=2.\n" );

    bool does_property_hold = false;
    for (long i = 0; i < arguments.size(); i++)
            does_property_hold |= AG_check_0thOrderLogic_property(arguments[i]);     

    return does_property_hold;
}

bool AG_notfn (vector<string> arguments) {
    if ( arguments.size() != 1 ) 
        abort_with_error_message( arguments.size() + " arguments to 'not' function instead of 1.\n" );
    
    return !(AG_check_0thOrderLogic_property( arguments[0] ) );   
}

bool AG_innfn (vector<string> arguments) {
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

bool AG_insfn (vector<string> arguments) {
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

bool AG_eqnfn (vector<string> arguments) {
    if ( arguments.size() != 2 ) 
        abort_with_error_message(arguments.size() + " arguments to 'eqn' function instead of 2.\n" );

    long first_number = stringToInt( arguments[0] ) ;
    long second_number = stringToInt( arguments[1] ) ;
    return (first_number == second_number);
}

bool AG_eqsfn (vector<string> arguments) {
    if ( arguments.size() != 2 ) 
        abort_with_error_message(arguments.size() + " arguments to 'eqs' function instead of 2.\n" );

    string str_first = arguments[0] ;
    string str_second = arguments[1] ;
    return (str_first.compare(str_second) == 0) ;
}

bool AG_impliesfn (vector<string> arguments) {
    if ( arguments.size() != 2 ) 
        abort_with_error_message( arguments.size() + " arguments to 'implies' function instead of 2.\n" );
    
    return (!(AG_check_0thOrderLogic_property( arguments[0] ) )  || (AG_check_0thOrderLogic_property( arguments[1] ) ) ) ;   
}


