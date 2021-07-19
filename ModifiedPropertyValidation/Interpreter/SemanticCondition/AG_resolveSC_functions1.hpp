#ifndef AGRESOLVESC_FUNCTIONS_HPP
#define AGRESOLVESC_FUNCTIONS_HPP

#include<fstream>
#include<vector>
#include<map>
#include<string>

using namespace std;

bool AG_issetfn(vector<string> arguments);

bool AG_isemptyarrfun(vector<string> arguments);

bool AG_trueConstant (vector<string> arguments);

bool AG_falseConstant (vector<string> arguments) ;

bool AG_andfn (vector<string> arguments) ;

bool AG_orfn (vector<string> arguments) ;

bool AG_notfn (vector<string> arguments);

bool AG_innfn (vector<string> arguments);

bool AG_insfn (vector<string> arguments);

bool AG_eqnfn (vector<string> arguments);

bool AG_eqsfn (vector<string> arguments);

bool AG_impliesfn (vector<string> arguments);

#endif