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
#include "../printFunctions.hpp"
//#include "resolveProperty.hpp"
#include "graphmacros1.hpp"
#include "resolveSemanticCondition1.hpp"
#include "resolveSemanticCondition_functions1.hpp"
#include "graphpropertyutility1.hpp"


using namespace std;

pair<string, string> get_dictName_and_dictArray (string property, long& quantifier_property_type, long& quantifier_type) {
    
    pair<string, string> dictName_and_dictArray = singleSplit( property, "in" );
    dictName_and_dictArray = trimWS_pair(dictName_and_dictArray);
    string dictName = trimWS((singleSplit(dictName_and_dictArray.first, " ")).second);
    string quantifier_type_string = (singleSplit(dictName_and_dictArray.first, " ")).first;
    if (quantifier_type_string == "forall")
        quantifier_type = 0;
    else if (quantifier_type_string == "exists")
        quantifier_type = 1;
    else
        abort_with_error_message("Expected 'forall' or 'exists' keyword as quantifier first word!\n");
    dictName_and_dictArray.first = dictName;
    quantifier_property_type = is_enclosed_by_delimiters(dictName, '(', ')') ? QUANTIFIER_EDGE_PROPERTY : QUANTIFIER_NODE_PROPERTY;
    return dictName_and_dictArray;
}

bool is_CHILD_reference1(string P) {
   string word= readwordfromstring(P);
   return (word == "Child") ;
}

vector<long> resolve_CHILD_reference1 (string P) {
    long ref_position = P.find("Child", 0);
    if (ref_position != 0)
        abort_with_error_message(P + "is not a Child reference!\n");
    long arg_start_position = P.find("(", ref_position  );
    if (arg_start_position == string::npos)
        abort_with_error_message("No arguments to Child reference!");
    long arg_end_position = P.find(")", arg_start_position );  
    if (arg_end_position == string::npos)
        abort_with_error_message("No terminating bracket to arguments of Child reference!");
    
    vector<string> arguments =  split( P.substr(arg_start_position + 1, arg_end_position - arg_start_position - 1), ","  ) ;
    if (arguments.size() != 1)
        abort_with_error_message("Incorrect number of arguments to Child reference!\n");
    
    long start_node_id = stringToInt(arguments[0]);

    return adjacencylist[ start_node_id];    
}


bool resolve_quantifier_properties(map<string,long >& quantifier_map, long recursion_depth , vector<string> quantifiers  ) {
    bool is_property_true;
    if (recursion_depth == quantifiers.size() - 1) {
        string zero_order_property = quantifiers[quantifiers.size() - 1];
        
        // cout << zero_order_property << endl; // Debugging!
        // cout << "Quantifier Map \n" ; // Debugging!
        // printMap(quantifier_map);     
        
        
        zero_order_property = resolve_all_node_references(zero_order_property, quantifier_map);
        zero_order_property = resolve_PATH_reference(zero_order_property);
        zero_order_property = resolve_PARENT_reference(zero_order_property);
        zero_order_property = resolve_GRANDPARENT_reference(zero_order_property);
        if (graph_type == "OT" )
            zero_order_property = resolve_ORDEREDCHILD_reference(zero_order_property); // reference to a predicate not possible for ordered trees


        is_property_true = graphSC_check_0thOrderLogic_property( zero_order_property);
        return is_property_true;
    }
    long quantifier_property_type = QUANTIFIER_UNCHECKED_PROPERTY;
    long quantifier_type ; // 0 for forall, 1 for exists;
    pair<string,string > dictName_and_dictArray = trimWS_pair( get_dictName_and_dictArray( quantifiers[ recursion_depth ], quantifier_property_type , quantifier_type) );
    is_property_true = (quantifier_type == 0) ;
    switch (quantifier_property_type)
    {
        case QUANTIFIER_NODE_PROPERTY: 
            {
                vector<long> nodes_to_iterate_over;                
                if (name_node_map.find(dictName_and_dictArray.second) == name_node_map.end() && !is_PATH_reference1(dictName_and_dictArray.second) )
                {
                    if (is_CHILD_reference1(dictName_and_dictArray.second)) {
                        string child_array = resolve_all_node_references( dictName_and_dictArray.second, quantifier_map ); // Originally this function  `AG_resolve_references' was used.
                        nodes_to_iterate_over = resolve_CHILD_reference1( child_array );
                    }
                    else if (dictName_and_dictArray.second == "Root")
                        nodes_to_iterate_over = Sources; 
                    else if (dictName_and_dictArray.second == "Leaves")
                        nodes_to_iterate_over = Leaves;
                    else if (dictName_and_dictArray.second == graph_type)
                        nodes_to_iterate_over = Graph;
                    else                    
                        abort_with_error_message("Quantifier node reference hasn't been defined by specification!\n");
                }
                else if (is_PATH_reference1(dictName_and_dictArray.second) ) {
                    dictName_and_dictArray.second = resolve_all_node_references(dictName_and_dictArray.second, quantifier_map);
                    nodes_to_iterate_over = get_PATH_reference(dictName_and_dictArray.second);
                }             
                else
                    nodes_to_iterate_over =  name_node_map[dictName_and_dictArray.second] ; 
                for (vector<long>::iterator it = nodes_to_iterate_over.begin() ; it != nodes_to_iterate_over.end() ; ++it) {
                    long node = *it;
                    quantifier_map.insert( pair<string, long> (dictName_and_dictArray.first, node) );
                    bool property_boolean_value = resolve_quantifier_properties(quantifier_map, recursion_depth + 1, quantifiers );
                    if (quantifier_type == 0)
                        is_property_true &= property_boolean_value;
                    else if (quantifier_type == 1)
                        is_property_true |= property_boolean_value;
                    quantifier_map.erase(dictName_and_dictArray.first);
                }
            }    
            break;
        case QUANTIFIER_EDGE_PROPERTY:
            {
                if (name_edge_map.find(dictName_and_dictArray.second) == name_edge_map.end() )
                    abort_with_error_message("Quantifier edge reference hasn't been defined by specification!\n");
                vector<pair<long,long> > edges_to_iterate_over = name_edge_map[dictName_and_dictArray.second];
                for (vector<pair<long,long> >::iterator it = edges_to_iterate_over.begin() ; it != edges_to_iterate_over.end() ; ++it) {
                    long node1 = it->first;
                    long node2 = it->second;
                    pair<string, string> dictNames = trimWS_pair(singleSplit( readstringbetween(  dictName_and_dictArray.first, '(', ')' ), ",") )  ;
                    quantifier_map.insert( pair<string, long> (dictNames.first, node1) );
                    quantifier_map.insert( pair<string, long> (dictNames.second, node2) );   
                    bool property_boolean_value = resolve_quantifier_properties(quantifier_map, recursion_depth + 1, quantifiers );
                    if (quantifier_type == 0)
                        is_property_true &= property_boolean_value;
                    else if (quantifier_type == 1)
                        is_property_true |= property_boolean_value;
                    quantifier_map.erase(dictNames.first);
                    quantifier_map.erase(dictNames.second);
                }
            }  
            break;
        default:
            abort_with_error_message("Unexpected property type for quantifier!\n");
            break;
    }
    return is_property_true;
}



bool graphSC_check_HigherOrderLogic_property (string P) {
    vector<string> quantifiers = trimWS_vector( split(P, ":") );
    map<string,long > quantifier_map;
    bool is_property_true = resolve_quantifier_properties(quantifier_map, 0, quantifiers );
    print_property_evaluation1( P, is_property_true);
    return is_property_true;
}


bool graphSC_check_0thOrderLogic_property ( string P) {
    string word= readwordfromstring(P);
    string argstring = readstringbetween_abort_iff_c1_and_no_c2(  P, '(', ')'); 
    vector<string> arguments = splitavoidarrdictfun(argstring, ","); 
    for (long i = 0; i < arguments.size(); i++) {
        arguments[i] = trimWS( arguments[i] );
    }

    if ( word == "and") 
        return graphSC_andfn(arguments); 
    else if ( word == "or") 
        return graphSC_orfn(arguments);
    else if ( word == "not") 
        return graphSC_notfn(arguments);
    else if ( word == "inn") 
        return graphSC_innfn(arguments);
    else if ( word == "ins")
        return graphSC_insfn(arguments);
    else if ( word == "eqn")
        return graphSC_eqnfn(arguments); 
    else if ( word == "eqs")
        return graphSC_eqsfn(arguments);
    else if (word == "True") 
        return graphSC_trueConstant(arguments);
    else if (word == "False") 
        return graphSC_falseConstant(arguments);   
    else if (word == "implies")
        return graphSC_impliesfn(arguments);  
    else if (word == "isempty")
        return graph_isemptyarrfun(arguments); 
    else if (word == "ancestor")
        return graph_ancestorfn(arguments);   
        else if (word == "l")
        return graphSC_l(arguments);
    else if (word == "le")
        return graphSC_le(arguments);
    else if (word == "g")
        return graphSC_g(arguments);
    else if (word == "ge")
        return graphSC_g(arguments);
    else if (word == "isempty_a")
        return AG_isemptyarrfun(arguments);  
    else if (word == "isset")
        return AG_issetfn(arguments);
    else 
        abort_with_error_message("Function " + word + " is not compatible with spec!\n"); 

    return false;
}

string resolve_all_node_references (string P, map<string,long > quantifier_map) {
    map<string, string> dict;
    string property = P;
    for (map<string,long >::iterator it = quantifier_map.begin(); it != quantifier_map.end(); ++it) {
        dict = dictionaries[ it->second ];
        property = resolve_SC_node_reference(it->first, property, dict);
    }
    return property;
}



string resolve_SC_node_reference (string var_name, string property, map<string, string> d) { 
    if (var_name.empty()) 
        abort_with_error_message("Empty name string provided here!\n");
    
    string property_without_references;
    string key;
    string value;
    for (long i = 0; i < property.length() ; i++) {
        if ( SC_have_reference1(property, i, var_name, key) ) {
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



string resolve_ORDEREDCHILD_reference( string P) {
    long search_start_position = 0;
    string edited_string = P;

    long ref_position = P.find("ochild_field", search_start_position);
    while (ref_position != string::npos) {
        long arg_start_position = P.find("(", ref_position  );
        if (arg_start_position == string::npos)
            abort_with_error_message("No arguments to grandparent_field reference!");
        long arg_end_position = P.find(")", arg_start_position );  
        if (arg_end_position == string::npos)
            abort_with_error_message("No terminating bracket to arguments of grandparent_field reference!");
        pair<string, string> argument_pair = trimWS_pair( singleSplit( P.substr(arg_start_position + 1, arg_end_position - arg_start_position - 1), ","  )) ;
        long node_id = stringToInt(argument_pair.first);
        string key = readstringbetween( argument_pair.second, '<' , '>' );
        
        long grandparent_node_id =  get_parent_node( get_parent_node( node_id) );
        if (grandparent_node_id == -1)
            abort_with_error_message("Can't get the grandparent of a node at depth atmost 1.\n");
        map<string, string> dict = dictionaries[ grandparent_node_id ];
        if ( dict.find(key) == dict.end() )
            abort_with_error_message(key + "field doesn't exist in grandparent dictionary.\n");
        string value = dict[ key ];
        edited_string.replace( edited_string.begin() + ref_position - 1  , edited_string.begin() + arg_end_position + 1  , value );        
        
        search_start_position = arg_end_position ;
        ref_position = P.find("ochild_field", search_start_position);
    }

    return edited_string;
}


string resolve_GRANDPARENT_reference(string P) {
    if (cycle == true )
        abort_with_error_message("grandparent_field is applicable only to acyclic graphs!.\n");
    
    long search_start_position = 0;
    string edited_string = P;

    long ref_position = P.find("grandparent_field", search_start_position);
    while (ref_position != string::npos) {
        long arg_start_position = P.find("(", ref_position  );
        if (arg_start_position == string::npos)
            abort_with_error_message("No arguments to grandparent_field reference!");
        long arg_end_position = P.find(")", arg_start_position );  
        if (arg_end_position == string::npos)
            abort_with_error_message("No terminating bracket to arguments of grandparent_field reference!");
        pair<string, string> argument_pair = trimWS_pair( singleSplit( P.substr(arg_start_position + 1, arg_end_position - arg_start_position - 1), ","  )) ;
        long node_id = stringToInt(argument_pair.first);
        string key = readstringbetween( argument_pair.second, '<' , '>' );
        
        long grandparent_node_id =  get_parent_node( get_parent_node( node_id) );
        if (grandparent_node_id == -1)
            abort_with_error_message("Can't get the grandparent of a node at depth atmost 1.\n");
        map<string, string> dict = dictionaries[ grandparent_node_id ];
        if ( dict.find(key) == dict.end() )
            abort_with_error_message(key + "field doesn't exist in grandparent dictionary.\n");
        string value = dict[ key ];
        edited_string.replace( edited_string.begin() + ref_position - 1  , edited_string.begin() + arg_end_position + 1  , value );        
        
        search_start_position = arg_end_position ;
        ref_position = P.find("grandparent_field", search_start_position);
    }

    return edited_string;
}


string resolve_PARENT_reference(string P) {
    if (cycle == true )
        abort_with_error_message("parent_field is applicable only to acyclic graphs!.\n");
    
    long search_start_position = 0;
    string edited_string = P;

    long ref_position = P.find("parent_field", search_start_position);
    while (ref_position != string::npos) {
        long arg_start_position = P.find("(", ref_position  );
        if (arg_start_position == string::npos)
            abort_with_error_message("No arguments to parent_field reference!");
        long arg_end_position = P.find(")", arg_start_position );  
        if (arg_end_position == string::npos)
            abort_with_error_message("No terminating bracket to arguments of parent_field reference!");
        pair<string, string> argument_pair = trimWS_pair( singleSplit( P.substr(arg_start_position + 1, arg_end_position - arg_start_position - 1), ","  )) ;
        long node_id = stringToInt(argument_pair.first);
        string key = readstringbetween( argument_pair.second, '<' , '>' );
        
        long parent_node_id = get_parent_node( node_id);
        if (parent_node_id == -1)
            abort_with_error_message("Can't get the parent of a source node.\n");
        map<string, string> dict = dictionaries[ parent_node_id ];
        if ( dict.find(key) == dict.end() )
            abort_with_error_message(key + "field doesn't exist in parent dictionary.\n");
        string value = dict[ key ];
        edited_string.replace( edited_string.begin() + ref_position - 1  , edited_string.begin() + arg_end_position + 1  , value );        
        
        search_start_position = arg_end_position ;
        ref_position = P.find("parent_field", search_start_position);
    }

    return edited_string;
}

long get_parent_node( long node_id) {
    for (auto it = adjacencylist.begin(); it != adjacencylist.end(); ++it) {
        long id = it->first;
        vector<long> kids = it->second;
        if ( is_element_in_Vector(kids, node_id) )
            return id;
    }
    return -1;
}

vector<long> get_PATH_reference (string P) {
    long ref_position = P.find("PATH", 0);
    if (ref_position != 0)
        abort_with_error_message(P + "is not a path reference!\n");
    long arg_start_position = P.find("(", ref_position  );
    if (arg_start_position == string::npos)
        abort_with_error_message("No arguments to PATH reference!");
    long arg_end_position = P.find(")", arg_start_position );  
    if (arg_end_position == string::npos)
        abort_with_error_message("No terminating bracket to arguments of PATH reference!");
    
    pair<string, string> argument_pair = trimWS_pair( singleSplit( P.substr(arg_start_position + 1, arg_end_position - arg_start_position - 1), ","  )) ;
    long start_node_id = stringToInt(argument_pair.first);
    string edge_type = argument_pair.second;
    map<long, vector<long> > all_edges;
    
    if (edge_type.compare(graph_type) == 0)
        all_edges = adjacencylist;
    else if ( name_edge_map.find(edge_type) != name_edge_map.end() )
        all_edges = set_of_edges_to_adjacency_list_representation1( name_edge_map[edge_type]  );
    else
        abort_with_error_message("Undefined edge type to PATH reference!\n");

    return BFS_nodes_visited1 (start_node_id, all_edges );    
}




string resolve_PATH_reference (string P) {
    
    long search_start_position = 0;
    string edited_string = P;

    long ref_position = P.find("PATH", search_start_position);
    while (ref_position != string::npos) {
        long arg_start_position = P.find("(", ref_position  );
        if (arg_start_position == string::npos)
            abort_with_error_message("No arguments to PATH reference!");
        long arg_end_position = P.find(")", arg_start_position );  
        if (arg_end_position == string::npos)
            abort_with_error_message("No terminating bracket to arguments of PATH reference!");
        pair<string, string> argument_pair = trimWS_pair( singleSplit( P.substr(arg_start_position + 1, arg_end_position - arg_start_position - 1), ","  )) ;
        long start_node_id = stringToInt(argument_pair.first);
        string edge_type = argument_pair.second;
        map<long, vector<long> > all_edges;
        if (edge_type.compare(graph_type) == 0)
            all_edges = adjacencylist;

        vector<long> edges = BFS_nodes_visited1 (start_node_id, all_edges );
        edited_string.replace( edited_string.begin() + ref_position - 1  , edited_string.begin() + arg_end_position + 1  , vector_of_Numbers_to_string( edges) );        
        
        search_start_position = arg_end_position ;
        ref_position = P.find("PATH", search_start_position);
    }

    return edited_string;

}



