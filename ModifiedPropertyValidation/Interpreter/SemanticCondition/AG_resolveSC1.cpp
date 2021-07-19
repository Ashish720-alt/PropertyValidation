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

bool solve_SC (string P) {
    vector<string> quantifiers = trimWS_vector( split(P, ":") );
    map<string,long > quantifier_map;
    bool propval = resolve_quantifiers_solveSC(quantifier_map, 0, quantifiers );
    return propval;
}


bool resolve_quantifiers_solveSC (map<string,long >& quantifier_map, long recursion_depth , vector<string> quantifiers  ) {
    
    if (recursion_depth == quantifiers.size() - 1) {
        string zero_order_property = quantifiers[quantifiers.size() - 1];
        zero_order_property = AG_resolve_references(zero_order_property, quantifier_map);
        if (zero_order_property.empty() ) 
            return false;
        return AG_check_0thOrderLogic_property ( zero_order_property );
    }
    pair<string,string > dictName_and_dictArray = trimWS_pair( AG_get_dictName_and_dictArray( quantifiers[ recursion_depth ] ) );
    
    vector<long> nodes_to_iterate_over;
    if (is_CHILD_reference(dictName_and_dictArray.second)) {
        string child_array = AG_resolve_references( dictName_and_dictArray.second, quantifier_map );
        nodes_to_iterate_over = resolve_CHILD_reference( child_array );
    }
    else if (dictName_and_dictArray.second == "Root")
        nodes_to_iterate_over = Sources; 
    else if (dictName_and_dictArray.second == "Leaves")
        nodes_to_iterate_over = Leaves;
    else if (dictName_and_dictArray.second == graph_type)
        nodes_to_iterate_over = Graph;
    else    
        abort_with_error_message("Quantifier node reference hasn't been defined by specification!\n");


    bool propval = true;
    for (vector<long>::iterator it = nodes_to_iterate_over.begin() ; it != nodes_to_iterate_over.end() ; ++it) {
        long node = *it;
        quantifier_map.insert( pair<string, long> (dictName_and_dictArray.first, node) );
        propval &= resolve_quantifiers_solveSC(quantifier_map, recursion_depth + 1, quantifiers );
        quantifier_map.erase(dictName_and_dictArray.first);
    }  

    return propval;
}


bool AG_check_0thOrderLogic_property ( string P) {
    string word= readwordfromstring(P);
    string argstring = readstringbetween_abort_iff_c1_and_no_c2(  P, '(', ')'); 
    vector<string> arguments =  trimWS_vector( splitavoidarrdictfun(argstring, ",") ); 

    if ( word == "and") 
        return AG_andfn(arguments); 
    else if ( word == "or") 
        return AG_orfn(arguments);
    else if ( word == "not") 
        return AG_notfn(arguments);
    else if ( word == "inn") 
        return AG_innfn(arguments);
    else if ( word == "ins")
        return AG_insfn(arguments);
    else if ( word == "eqn")
        return AG_eqnfn(arguments); 
    else if ( word == "eqs")
        return AG_eqsfn(arguments);
    else if (word == "True") 
        return AG_trueConstant(arguments);
    else if (word == "False") 
        return AG_falseConstant(arguments);   
    else if (word == "implies")
        return AG_impliesfn(arguments);  
    else if (word == "isempty_a")
        return AG_isemptyarrfun(arguments);  
    else if (word == "isset")
        return AG_issetfn(arguments);
    else 
        abort_with_error_message("Function " + word + " is not compatible with spec!\n"); 

    return false;
}

