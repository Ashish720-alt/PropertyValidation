#ifndef RESOLVESEMANTICCONDITION_HPP
#define RESOLVESEMANTICCONDITION_HPP

#include<fstream>
#include <vector>
#include <string>
#include <map>


using namespace std;

string resolve_ORDEREDCHILD_reference( string P);

string resolve_GRANDPARENT_reference(string P);

string resolve_PARENT_reference(string P) ;

long get_parent_node( long node_id);

string resolve_PATH_reference (string P);

vector<long> get_PATH_reference (string P);

bool graphSC_check_0thOrderLogic_property ( string P) ;

bool graphSC_check_HigherOrderLogic_property (string P);

string resolve_all_node_references (string P, map<string,long > quantifier_map);

pair<string, string> get_dictName_and_dictArray (string property, long& quantifier_property_type, long& quantifer_type);

bool is_CHILD_reference(string P);

vector<long> resolve_CHILD_reference (string P);

bool resolve_quantifier_properties(map<string,long >& quantifier_map, long recursion_depth , vector<string> quantifiers  );

long graphSC_findChildIndex ( string P);

long graphSC_indexnfn (vector<string> args);

long graphSC_indexsfn (vector<string> args);

string resolve_SC_node_reference (string var_name, string property, map<string, string> d);

string resolve_retvalFunction_reference (struct property_data PD);

bool have_retvalFunction_reference (string p, long& pos, string function_name, string& argstring ) ;

#endif

