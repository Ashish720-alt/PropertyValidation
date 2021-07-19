#ifndef RESOLVESEMANTICCONDITION_FUNCTIONS_HPP
#define RESOLVESEMANTICCONDITION_FUNCTIONS_HPP

#include<fstream>
#include <vector>
#include <string>
#include <map>


using namespace std;

bool graph_ancestorfn(vector<string> arguments);

bool graph_isemptyarrfun(vector<string> arguments) ;

bool graphSC_trueConstant (vector<string> arguments);

bool graphSC_falseConstant (vector<string> arguments);

bool graphSC_andfn (vector<string> args);

bool graphSC_orfn (vector<string> args);

bool graphSC_notfn (vector<string> args);

bool graphSC_innfn (vector<string> args) ;

bool graphSC_insfn (vector<string> args);

bool graphSC_eqnfn (vector<string> args);

bool graphSC_eqsfn (vector<string> args);

bool graphSC_impliesfn (vector<string> arguments);

bool graphSC_l(vector<string> arguments);

bool graphSC_le(vector<string> arguments);

bool graphSC_g(vector<string> arguments);

bool graphSC_ge(vector<string> arguments);

bool AG_issetfn(vector<string> arguments);
bool AG_isemptyarrfun(vector<string> arguments) ;

#endif
