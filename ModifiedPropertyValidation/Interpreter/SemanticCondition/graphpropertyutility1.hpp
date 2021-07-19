#ifndef GRAPHPROPERTYUTILITY_HPP
#define GRAPHPROPERTYUTILITY_HPP

#include<fstream>
#include <vector>
#include <string>
#include <map>


using namespace std;

void print_property_evaluation1 (string P, bool is_property_true);

bool SC_have_reference1 (string P, long& pos, string name, string& key  );

map<long, vector<long> > set_of_edges_to_adjacency_list_representation1( vector<pair<long, long> > edges  );

vector<long> BFS_nodes_visited1 (long source, map<long, vector<long> > adj_list);

bool is_PATH_reference1(string P);

bool have_reference1 (string P, long& pos, string name, string& key  );

string resolve_retvalFunction_reference1 (struct property_data PD);

bool have_retvalFunction_reference1 (string p, long& pos, string function_name, string& argstring ) ;

#endif