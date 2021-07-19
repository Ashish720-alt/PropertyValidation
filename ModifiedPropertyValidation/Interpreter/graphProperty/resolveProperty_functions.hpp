#ifndef RESOLVEPROPERTY_FUNCTIONS_HPP
#define RESOLVEPROPERTY_FUNCTIONS_HPP

#include<fstream>
#include <vector>
#include <string>
#include <map>


using namespace std;

bool graph_trueConstant (vector<string> arguments);

bool graph_falseConstant (vector<string> arguments);

bool graph_andfn (vector<string> args);

bool graph_orfn (vector<string> args);

bool graph_notfn (vector<string> args);

bool graph_innfn (vector<string> args) ;

bool graph_insfn (vector<string> args);

bool graph_eqnfn (vector<string> args);

bool graph_eqsfn (vector<string> args);

bool graph_implies (vector<string> arguments);

bool graph_l(vector<string> arguments);

bool graph_le(vector<string> arguments);

bool graph_g(vector<string> arguments);

bool graph_ge(vector<string> arguments);

#endif