#ifndef AGUTILITY_HPP
#define AGUTILITY_HPP

#include<fstream>
#include<vector>
#include<map>
#include<string>

using namespace std;


vector<long> resolve_CHILD_reference (string P) ;

bool is_CHILD_reference(string P) ;

pair<string, string> AG_get_dictName_and_dictArray (string property);


string AG_resolve_references (string P, map<string,long > quantifier_map);

string AG_resolve_single_reference (string var_name, string property, map<string, string> d);

bool AG_have_reference (string p, long& pos, string name, string& key  ) ;

string read_AG_property_without_delimiters (fstream& f);

void print_prop (string P, bool propval);

void print_all_attributes () ;

#endif