#include<iostream>
#include<set>
#include<vector>
#include<map>
#include<string>
#include<fstream>
#include<stdlib.h>
#include "../macros.hpp"
#include "../utility.hpp"
#include "../printFunctions.hpp"
#include "graphPropertySpec.hpp"
#include "graphmacros.hpp"
#include "resolveProperty.hpp"

using namespace std;

map<string, vector<long> > name_node_map;  
map<string, vector< pair<long, long> > > name_edge_map; 

void graph_property_specification ( fstream& spec_file ) {
    long propertyType;
    struct property_data prop_data;
    string single_spec = read_single_spec_without_delimiters_and_abort_if_wrong_format(spec_file);  
    extract_property_data( single_spec, prop_data );        
    switch ( prop_data.type ) {
        case NODE_PROPERTY: 
            {
                vector<long> nodes = resolve_nodes(prop_data);
                name_node_map.insert ( pair<string, vector<long> >( prop_data.name, nodes ) );
            }
            break;
        case EDGE_PROPERTY: 
            {
                vector< pair<long, long> > edges = resolve_edges(prop_data);
                name_edge_map.insert ( pair<string, vector< pair<long, long> > > ( prop_data.name, edges ) );
            }  
            break;
        default:
            abort_with_error_message( "Unexpected property type returned!\n");
            break;
    } 




    return;
}

vector< pair<long, long> > resolve_edges(struct property_data PD) {
    string property_with_references = PD.property;
    vector<string> property_split_at_where = split( property_with_references, "where" );
    if (property_split_at_where.size() != 3)
        abort_with_error_message("In edge property, expect exactly 2 where conditions, instead of " + to_string(property_split_at_where.size()) + " .\n" );
    pair<string, string> var_name = trimWS_pair( singleSplit( readstringbetween( PD.variable_name, '(', ')') , "," ) ) ;
    pair<string, string> dict_list_pair1 =  trimWS_pair( singleSplit( trimWS(property_split_at_where[1]) , "in" ) );
    pair<string, string> dict_list_pair2 =  trimWS_pair( singleSplit( trimWS(property_split_at_where[2]) , "in" ) );    

    vector<long> ids_1, ids_2;
    if (var_name.first == dict_list_pair1.first && var_name.second == dict_list_pair2.first) {
        ids_1 = get_ids_to_iterate_over ( dict_list_pair1.second  ) ;
        ids_2 = get_ids_to_iterate_over ( dict_list_pair2.second  ) ;
    }
    else if (var_name.first == dict_list_pair2.first && var_name.second == dict_list_pair1.first) {
        ids_1 = get_ids_to_iterate_over ( dict_list_pair2.second  ) ;
        ids_2 = get_ids_to_iterate_over ( dict_list_pair1.second  ) ;
    }
    else
        abort_with_error_message("Variable names don't match in edge property.\n");

    vector< pair<long, long> > edges;
    map<string, string> d1, d2;
    string no_reference_property;
    string partially_resolved_reference_property;
    bool is_property_true;
    for (long i = 0; i < ids_1.size(); i++) {
        d1 = dictionaries[ Graph[ ids_1[i] ] ];
        PD.property = property_split_at_where[0];
        PD.variable_name = var_name.first;

        partially_resolved_reference_property = resolve_node_reference(PD, d1);
        if (partially_resolved_reference_property.empty())
            continue; 
     
        PD.variable_name = var_name.second;      
        
        for (long j = 0; j < ids_2.size(); j++) {
            d2 = dictionaries[ Graph[ ids_2[j] ] ];
            PD.property = partially_resolved_reference_property;            
            no_reference_property = resolve_node_reference(PD, d2);
            if (no_reference_property.empty())
                continue;
            PD.property = no_reference_property; 
            no_reference_property = resolve_retvalFunction_reference(PD); 
            is_property_true = graph_checkproperty( no_reference_property );
            if (is_property_true)
                edges.push_back( pair<long, long> ( ids_1[i] , ids_2[j] )  ) ;   
        }
    }
    return edges;

}

vector<long> resolve_nodes(struct property_data PD) {
    vector<long> nodes;
    long id;
    map <string, string> node;
    string no_reference_property;
    bool is_property_true;
    bool have_where_clause;


    vector<string> property_split_at_where = trimWS_vector( split( PD.property, "where" ));
    
    

    have_where_clause = (property_split_at_where.size() > 1);
    PD.property = property_split_at_where[0];
    vector< vector<long> > nodes_iterate_array;
    vector<string> node_names_array;

    for (long i = 1; i < property_split_at_where.size(); i++) {
        pair<string, string> single_where_clause = singleSplit( property_split_at_where[i], " in " );
        node_names_array.push_back( single_where_clause.first );



        vector<long> nodes_to_iterate_over = get_ids_to_iterate_over( single_where_clause.second );


        nodes_iterate_array.push_back(nodes_to_iterate_over);
    }



    for (vector<long>::iterator it = Graph.begin(); it != Graph.end(); it++) {
        id = *it;
        node = dictionaries[id];
        no_reference_property = resolve_node_reference(PD, node);
        if (no_reference_property.empty())
            continue;
        
        if(have_where_clause) {
            is_property_true = true;
            struct property_data PD_where;

            for(vector<long> index = initialize_to_smallest_index(nodes_iterate_array) ;  ; index = next_node(index, nodes_iterate_array) ) {
                bool string_empty = false;
                for (long j = 0 ; j < index.size() ; j++) {
                    PD_where.property = no_reference_property;
                    PD_where.variable_name = node_names_array[j];
                    string modified_property = resolve_node_reference(PD_where, dictionaries[ index[j] ] );    
                    if (modified_property.empty()) {
                        string_empty = true;
                        break;
                    }
                    no_reference_property = modified_property;    
                }
                if (string_empty)
                    continue;
                bool check_prop = graph_checkproperty(no_reference_property);
                is_property_true &= check_prop;

                if (is_max_lexicographic_index(index, nodes_iterate_array) )
                    break;
            }
            
        }
        else {
            is_property_true = graph_checkproperty( no_reference_property );
        }
        if (is_property_true)
            nodes.push_back(id);
    }

    return nodes;
}


vector<long> initialize_to_smallest_index ( vector< vector<long> > all_nodes) {
    vector<long> nodes;
    long count = all_nodes.size();
    for (long j = 0; j < count ; j++) {
        vector<long> nodes_to_iterate_over = all_nodes[j];
        nodes.push_back( nodes_to_iterate_over[0] );
    }   
    return nodes;
}

bool is_max_lexicographic_index( vector<long> index, vector< vector<long> > all_nodes  ) {
    long count = all_nodes.size();
    for (long i = 0 ; i < count; i++) {
        vector<long> nodes_to_iterate_over = all_nodes[i];
        long local_count = nodes_to_iterate_over.size();
        if (index[i] != nodes_to_iterate_over[local_count - 1])
            return false;
    }
    return true;
}

vector<long> next_node( vector<long> index, vector< vector<long> > all_nodes  ) {
    vector<long> rv = index;
    long count = all_nodes.size();
    long position_of_change;
    for (long i = count - 1 ; i >= 0 ; i--) {
        vector<long> nodes_to_iterate_over = all_nodes[i];
        long local_count = nodes_to_iterate_over.size();
        if (index[i] != nodes_to_iterate_over[local_count - 1]) {
            position_of_change = i;
            break;
        }
    }

     // Change at position_of_change
    {
        vector<long> nodes_to_iterate_over = all_nodes[position_of_change];
        for (long i = 0 ; i < nodes_to_iterate_over.size() ; i++ ) {
            if ( nodes_to_iterate_over[i] == index[position_of_change]) {
                rv[position_of_change] = nodes_to_iterate_over[i+1];
                break;
            }
        }
    }

    // Change after position_of_change
    for (long i = position_of_change + 1; i < count ; i++) {
        vector<long> nodes_to_iterate_over = all_nodes[i];
        rv[i] = nodes_to_iterate_over[0];
    }

    return rv;
} 


vector<long> get_ids_to_iterate_over (string s) {
    vector<long> retval;
    vector<long> vector_to_append;
    vector<string> id_labels = trimWS_vector (split( readstringbetween ( s, '[', ']'), "," ) );
    for (long i = 0; i < id_labels.size(); i++) {
        if (id_labels[i] == graph_type) {
            vector_to_append = Graph ;
            retval.insert( retval.end(), vector_to_append.begin() , vector_to_append.end() ); 
        }
        else if (name_node_map.find( id_labels[i]) != name_node_map.end()) {
            vector_to_append = name_node_map[ id_labels[i] ];
            retval.insert( retval.end(), vector_to_append.begin() , vector_to_append.end() ); 
        }
        else 
            abort_with_error_message("Labels to iterate over are unknown.\n");
    }
    return retval;
}

void extract_property_data(string s, struct property_data& D) {
    string text, property;
    pair<string, string> text_and_property = singleSplit(s, "satisfying");
    text = trimWS( text_and_property.first );
    property = trimWS (text_and_property.second);

    string name, variablename;
    pair<string, string> name_and_variablename = singleSplit(text, "is");
    name = trimWS(name_and_variablename.first);
    variablename = trimWS (name_and_variablename.second);

    long property_type = UNCHECKED_PROPERTY;
    bool is_bracket_enclosed = is_enclosed_by_delimiters (variablename, '(', ')');
    property_type = (is_bracket_enclosed) ? (EDGE_PROPERTY) : (NODE_PROPERTY);

    D.name = name;
    D.variable_name = variablename;
    D.property = property;
    D.type = property_type;

    return;
}

string read_single_spec_without_delimiters_and_abort_if_wrong_format (fstream& f) {
    string property;
    char c = getcharignoreWSEOFabort( f);
    while (  c != PROPERTY_END_DELIMITING_CHARACTAR || f.peek() != PROPERTY_END_DELIMITING_CHARACTAR ) {
        property.append(1, c);
        c = charEOFabort(f);
    }
    charcompareabort(f, PROPERTY_END_DELIMITING_CHARACTAR);
    return property;
}

string read_SC_spec_without_delimiters_and_abort_if_wrong_format (fstream& f) {
    string SC;
    char c = getcharignoreWSEOFabort( f);
    while (  c != PROPERTY_END_DELIMITING_CHARACTAR || f.peek() != PROPERTY_END_DELIMITING_CHARACTAR ) {
        SC.append(1, c);
        c = charEOFabort(f);
    }
    charcompareabort(f, PROPERTY_END_DELIMITING_CHARACTAR);
    return SC;
}
