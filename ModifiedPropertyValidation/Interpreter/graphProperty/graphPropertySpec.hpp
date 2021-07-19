#ifndef GRAPHPROPERTYSPEC_HPP
#define GRAPHPROPERTYSPEC_HPP

#include<fstream>
#include<vector>
#include<map>
#include<string>
#include "../macros.hpp"

using namespace std;

void graph_property_specification ( fstream& specF );

 void extract_property_data(string s, struct property_data& D)  ;

string read_single_spec_without_delimiters_and_abort_if_wrong_format (fstream& f);

string read_SC_spec_without_delimiters_and_abort_if_wrong_format (fstream& f) ;

vector<long> resolve_nodes(struct property_data PD);

vector< pair<long, long> > resolve_edges(struct property_data PD);

vector<long> get_ids_to_iterate_over (string s);

vector<long> initialize_to_smallest_index ( vector< vector<long> > all_nodes) ;

bool is_max_lexicographic_index( vector<long> index, vector< vector<long> > all_nodes  );

vector<long> next_node( vector<long> index, vector< vector<long> > all_nodes  );


#endif

/****************************************************************************
Function and Return Value: 
****************************************************************************/