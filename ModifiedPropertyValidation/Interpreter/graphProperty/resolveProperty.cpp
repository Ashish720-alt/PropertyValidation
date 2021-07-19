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
#include "graphmacros.hpp"
#include "../printFunctions.hpp"
#include "resolveProperty_functions.hpp"
#include "graphpropertyutility.hpp"

using namespace std;


bool graph_checkproperty ( string P) {
    string word= readwordfromstring(P);
    string argstring = readstringbetween_abort_iff_c1_and_no_c2(  P, '(', ')'); 
    vector<string> arguments = splitavoidarrdictfun(argstring, ","); 
    
    for (long i = 0; i < arguments.size(); i++) {
        arguments[i] = trimWS( arguments[i] );
    }

    if ( word == "and") 
        return graph_andfn(arguments); 
    else if ( word == "or") 
        return graph_orfn(arguments);
    else if ( word == "not") 
        return graph_notfn(arguments);
    else if ( word == "inn") 
        return graph_innfn(arguments);
    else if ( word == "ins")
        return graph_insfn(arguments);
    else if ( word == "eqn")
        return graph_eqnfn(arguments); 
    else if ( word == "eqs")
        return graph_eqsfn(arguments);
    else if (word == "True") 
        return graph_trueConstant(arguments);
    else if (word == "False") 
        return graph_falseConstant(arguments);
    else if (word == "implies")
        return graph_implies(arguments);   
    else if (word == "l")
        return graph_l(arguments);
    else if (word == "le")
        return graph_le(arguments);
    else if (word == "g")
        return graph_g(arguments);
    else if (word == "ge")
        return graph_g(arguments);
    else 
        abort_with_error_message("Function " + word + " is not compatible with spec!\n"); 

    return false;
}

string resolve_node_reference (struct property_data PD, map<string, string> d) { 
    string var_name = PD.variable_name;
    string property = PD.property;
    if (var_name.empty()) 
        abort_with_error_message("Empty name string provided here!\n");
    
    string property_without_references;
    string key;
    string value;
    for (long i = 0; i < property.length() ; i++) {
        if ( have_reference(property, i, var_name, key) ) {
            if ( d.find(key) == d.end() )
                return "";
            value = d[key];
            property_without_references.append(value);
            continue;
        }
        else 
            property_without_references.push_back(property[i]);
    }

    return property_without_references;
}




