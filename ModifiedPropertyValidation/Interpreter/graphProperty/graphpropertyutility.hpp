#ifndef GRAPHPROPERTYUTILITY_HPP
#define GRAPHPROPERTYUTILITY_HPP

#include<fstream>
#include <vector>
#include <string>
#include <map>


using namespace std;

void print_property_evaluation (string P, bool is_property_true);

bool SC_have_reference (string P, long& pos, string name, string& key  );

map<long, vector<long> > set_of_edges_to_adjacency_list_representation( vector<pair<long, long> > edges  );

vector<long> BFS_nodes_visited (long source, map<long, vector<long> > adj_list);

bool is_PATH_reference(string P);

bool have_reference (string P, long& pos, string name, string& key  );

string resolve_retvalFunction_reference (struct property_data PD);

bool have_retvalFunction_reference (string p, long& pos, string function_name, string& argstring ) ;

#endif