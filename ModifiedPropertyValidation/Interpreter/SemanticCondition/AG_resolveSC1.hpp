#ifndef AGRESOLVESC_HPP
#define AGRESOLVESC_HPP

#include<fstream>
#include<vector>
#include<map>
#include<string>

using namespace std;

bool solve_SC (string P) ;

bool resolve_quantifiers_solveSC (map<string,long >& quantifier_map, long recursion_depth , vector<string> quantifiers  ) ;

bool AG_check_0thOrderLogic_property ( string P);

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