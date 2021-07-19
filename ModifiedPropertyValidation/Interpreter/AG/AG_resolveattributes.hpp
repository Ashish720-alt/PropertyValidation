#ifndef AGRESOLVEATTRIBUTES_HPP
#define AGRESOLVEATTRIBUTES_HPP

#include<fstream>
#include<vector>
#include<map>
#include<string>

using namespace std;

void find_attribute (string P);

bool resolve_quantifiers_find_attribute (map<string,long >& quantifier_map, long recursion_depth , vector<string> quantifiers  );

bool solve_attribute_equation (string P, map<string, long> quantifier_map);

string solve_attribute_value ( string P) ;

#endif