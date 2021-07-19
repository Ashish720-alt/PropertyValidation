#ifndef AGRESOLVEATTRIBUTES_FUNCTIONS_HPP
#define AGRESOLVEATTRIBUTES_FUNCTIONS_HPP

#include<fstream>
#include<vector>
#include<map>
#include<string>

using namespace std;

string AG_make_singleton_array(vector<string> arguments);

string AG_dictval(vector<string> arguments);

string AG_union(vector<string> arguments) ;

string AG_appendallChildren(vector<string> arguments);

string string_andfn (vector<string> args) ;

string string_orfn (vector<string> args);

string string_notfn (vector<string> args);

string iskeydef (vector<string> args);



#endif