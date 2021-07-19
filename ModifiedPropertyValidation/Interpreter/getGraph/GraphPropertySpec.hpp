#ifndef GRAPHPROPERTY_HPP
#define GRAPHPROPERTY_HPP

#include<fstream>
#include <vector>
#include <string>
#include <map>


using namespace std;

bool checkproperty ( string P) ;


bool andfn (vector<string> args);


bool orfn (vector<string> args);


bool notfn (vector<string> args);


bool innfn (vector<string> args) ;


bool insfn (vector<string> args);


bool eqnfn (vector<string> args);

bool eqsfn (vector<string> args);

long findChildIndex ( string P);


long indexnfn (vector<string> args);


long indexsfn (vector<string> args);

#endif

