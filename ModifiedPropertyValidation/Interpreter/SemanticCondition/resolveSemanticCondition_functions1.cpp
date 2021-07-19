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
//#include "resolveProperty.hpp"
#include "graphmacros1.hpp"
#include "resolveSemanticCondition1.hpp"
#include "resolveSemanticCondition_functions1.hpp"
#include "graphpropertyutility1.hpp"

using namespace std;

bool graphSC_l(vector<string> arguments) {
    if ( arguments.size() != 2 ) 
        abort_with_error_message( arguments.size() + " arguments to constant value < instead of 2.\n" );      
    long num1 = stringToInt( arguments[0] ) ;
    long num2 = stringToInt( arguments[1] ) ;
    return (num1 < num2);
}

bool graphSC_le(vector<string> arguments) {
    if ( arguments.size() != 2 ) 
        abort_with_error_message( arguments.size() + " arguments to constant value <= instead of 2.\n" );      
    long num1 = stringToInt( arguments[0] ) ;
    long num2 = stringToInt( arguments[1] ) ;
    return (num1 <= num2);
}

bool graphSC_g(vector<string> arguments) {
    if ( arguments.size() != 2 ) 
        abort_with_error_message( arguments.size() + " arguments to constant value > instead of 2.\n" );      
    long num1 = stringToInt( arguments[0] ) ;
    long num2 = stringToInt( arguments[1] ) ;
    return (num1 > num2);
}

bool graphSC_ge(vector<string> arguments) {
    if ( arguments.size() != 2 ) 
        abort_with_error_message( arguments.size() + " arguments to constant value >= instead of 2.\n" );      
    long num1 = stringToInt( arguments[0] ) ;
    long num2 = stringToInt( arguments[1] ) ;
    return (num1 >= num2);
}



bool graph_ancestorfn(vector<string> arguments) {
    if ( arguments.size() != 2 ) 
        abort_with_error_message( arguments.size() + " arguments to ancestor function instead of 2.\n" );    
    long descendant_id = stringToInt (arguments[0] );
    long ancestor_id = stringToInt( arguments[1] );
    vector<long> all_descendant_nodes = BFS_nodes_visited1 ( ancestor_id, adjacencylist);
    return ( is_element_in_Vector(all_descendant_nodes, descendant_id) && ancestor_id != descendant_id );
}


bool graph_isemptyarrfun(vector<string> arguments) {
    if ( arguments.size() != 1 ) 
        abort_with_error_message( arguments.size() + " arguments to isempty_a function instead of 1.\n" );     
    string array_contents = trimWS( readstringbetween( arguments[0], '[', ']' ) );
    return (array_contents.empty()) ;
}


bool graphSC_trueConstant (vector<string> arguments) {
    if ( arguments.size() != 0 ) 
        abort_with_error_message( arguments.size() + " arguments to constant value \"True\" instead of 0.\n" );    

    return true;
}

bool graphSC_falseConstant (vector<string> arguments) {
    if ( arguments.size() != 0 ) 
        abort_with_error_message( arguments.size() + " arguments to constant value \"False\" instead of 0.\n" );    

    return false;
}

bool graphSC_andfn (vector<string> arguments) {
    if ( arguments.size() <= 1 ) 
        abort_with_error_message( arguments.size() + " arguments to 'and' function instead of >=2.\n" );

    bool does_property_hold = true;
    for (long i = 0; i < arguments.size(); i++) 
        does_property_hold &= graphSC_check_0thOrderLogic_property(arguments[i]);    

    return does_property_hold;
}

bool graphSC_orfn (vector<string> arguments) {
    if ( arguments.size() <= 1 ) 
            abort_with_error_message( arguments.size() + " arguments to 'or' function instead of >=2.\n" );

    bool does_property_hold = false;
    for (long i = 0; i < arguments.size(); i++)
            does_property_hold |= graphSC_check_0thOrderLogic_property(arguments[i]);     

    return does_property_hold;
}

bool graphSC_notfn (vector<string> arguments) {
    if ( arguments.size() != 1 ) 
        abort_with_error_message( arguments.size() + " arguments to 'not' function instead of 1.\n" );
    
    return !(graphSC_check_0thOrderLogic_property( arguments[0] ) );   
}

bool graphSC_innfn (vector<string> arguments) {
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

bool graphSC_insfn (vector<string> arguments) {
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

bool graphSC_eqnfn (vector<string> arguments) {
    if ( arguments.size() != 2 ) 
        abort_with_error_message(arguments.size() + " arguments to 'eqn' function instead of 2.\n" );

    long first_number = stringToInt( arguments[0] ) ;
    long second_number = stringToInt( arguments[1] ) ;
    return (first_number == second_number);
}

bool graphSC_eqsfn (vector<string> arguments) {
    if ( arguments.size() != 2 ) 
        abort_with_error_message(arguments.size() + " arguments to 'eqs' function instead of 2.\n" );

    string str_first = arguments[0] ;
    string str_second = arguments[1] ;
    return (str_first.compare(str_second) == 0) ;
}

bool graphSC_impliesfn (vector<string> arguments) {
    if ( arguments.size() != 2 ) 
        abort_with_error_message( arguments.size() + " arguments to 'implies' function instead of 2.\n" );
    
    return (!(graphSC_check_0thOrderLogic_property( arguments[0] ) )  || (graphSC_check_0thOrderLogic_property( arguments[1] ) ) ) ;   
}

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

