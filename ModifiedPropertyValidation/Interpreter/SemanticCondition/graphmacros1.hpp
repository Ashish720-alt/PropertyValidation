#ifndef GRAPHMACROS_HPP
#define GRAPHMACROS_HPP

using namespace std;

#define SPECIFICATION_DELIMITER '-'
#define SEMANTIC_CONDITION_DELIMITER '%'
#define PROPERTY_END_DELIMITING_CHARACTAR '.'

#define UNCHECKED_PROPERTY 0
#define NODE_PROPERTY 1
#define EDGE_PROPERTY 2
#define SC_PROPERTY 3

#define QUANTIFIER_UNCHECKED_PROPERTY -1
#define QUANTIFIER_NODE_PROPERTY 0
#define QUANTIFIER_EDGE_PROPERTY 1


struct property_data {
    string name;
    string variable_name;
    string property;
    long type;
};

extern map<string, vector<long> > name_node_map;  
extern map<string, vector< pair<long, long> > > name_edge_map; 

#endif