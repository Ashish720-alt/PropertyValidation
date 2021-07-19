#include<iostream>
#include<set>
#include<vector>
#include<map>
#include<string>
#include<fstream>
#include<stdlib.h>
#include "AGspec.hpp"
#include "AGmacros.hpp"
#include "AGutility.hpp"
#include "AG_resolveattributes.hpp"
#include "../macros.hpp"
#include "../utility.hpp"
#include "../printFunctions.hpp"
#include "AG_resolveattributes_functions.hpp"


using namespace std;


void find_attribute (string P) {
    vector<string> quantifiers = trimWS_vector( split(P, ":") );
    
    map<string,long > quantifier_map;
    resolve_quantifiers_find_attribute(quantifier_map, 0, quantifiers );

    return;
}


bool resolve_quantifiers_find_attribute (map<string,long >& quantifier_map, long recursion_depth , vector<string> quantifiers  ) {
    bool has_value_changed = false;
    
    // Base Case of recursion: Check if the equation holds for specific values over quantified variables.
    if (recursion_depth == quantifiers.size() - 1) {
        string zero_order_property = quantifiers[quantifiers.size() - 1];
        has_value_changed = solve_attribute_equation( zero_order_property, quantifier_map);
        return has_value_changed;
    }
    pair<string,string > dictName_and_dictArray = trimWS_pair( AG_get_dictName_and_dictArray( quantifiers[ recursion_depth ] ) );
    
    // Quantifiers over attribute definitions can be over Roots, Leaves, Tree, or Children of some dictionary which is previously quantified (i.e. CHILD(d)).
    vector<long> nodes_to_iterate_over;
    if (is_CHILD_reference(dictName_and_dictArray.second)) {
        string child_array = AG_resolve_references( dictName_and_dictArray.second, quantifier_map );
        nodes_to_iterate_over = (!child_array.empty()) ? resolve_CHILD_reference( child_array ) : vector<long> ();
    }
    else if (dictName_and_dictArray.second == "Root")
        nodes_to_iterate_over = Sources; 
    else if (dictName_and_dictArray.second == "Leaves")
        nodes_to_iterate_over = Leaves;
    else if (dictName_and_dictArray.second == graph_type)
        nodes_to_iterate_over = Graph;
    else    
        abort_with_error_message("Quantifier node reference hasn't been defined by specification!\n");

    do {
        has_value_changed = false;
        for (vector<long>::iterator it = nodes_to_iterate_over.begin() ; it != nodes_to_iterate_over.end() ; ++it) {
            long node = *it;
            quantifier_map.insert( pair<string, long> (dictName_and_dictArray.first, node) );
            has_value_changed |= resolve_quantifiers_find_attribute(quantifier_map, recursion_depth + 1, quantifiers );
            quantifier_map.erase(dictName_and_dictArray.first);
        }
    } while ( has_value_changed ); 

    return has_value_changed;
}

bool solve_attribute_equation (string P, map<string, long> quantifier_map) {
    pair<string, string> eqn_split_at_equalto = trimWS_pair( singleSplit( P, "=" )  );
    eqn_split_at_equalto.second = AG_resolve_references( eqn_split_at_equalto.second, quantifier_map );
    string node =  readwordfromstring( eqn_split_at_equalto.first ) ;
    string attr_name = readstringbetween( eqn_split_at_equalto.first, '<' , '>' );
    long ID = quantifier_map[ node ];
    map<string, string> dict = dictionaries [ ID  ];
    string attr_val = (eqn_split_at_equalto.second != "") ? solve_attribute_value ( eqn_split_at_equalto.second) : "" ;
    bool has_value_changed = !(dict.find(attr_name) != dict.end() && dict[attr_name] == attr_val ) ;

    if (!has_value_changed)
        return false;
    dict.erase( attr_name );
    dict.insert( pair<string, string>(attr_name, attr_val) );
    dictionaries.erase( ID );
    dictionaries.insert( pair<long, map<string, string> >(ID, dict) );
    return has_value_changed;
}

string solve_attribute_value ( string P)  {
    string word = readwordfromstring(P);
    string argstring = readstringbetween(  P, '(', ')'); 
    vector<string> args =  trimWS_vector( splitavoidarrdictfun(argstring, ",") ); 

    if ( word == "and") 
        return string_andfn(args); 
    else if ( word == "or") 
        return string_orfn(args);
    else if ( word == "not") 
        return string_notfn(args);
    else if ( word == "iskeydefined")
        return iskeydef(args);
    else if (word == "dictval")
        return AG_dictval(args);
    else if (word == "union")
        return AG_union(args);
    else if (word == "append_all_children_attributes")
        return AG_appendallChildren(args);
    else if (word == "make_singleton_array")
        return AG_make_singleton_array(args);
    else 
        abort_with_error_message("Function " + word + " is not compatible with spec!\n");

    return "";
}





