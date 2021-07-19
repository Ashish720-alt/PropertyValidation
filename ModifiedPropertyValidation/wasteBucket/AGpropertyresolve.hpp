#ifndef AGPROPERTYRESOLVE_HPP
#define AGPROPERTYRESOLVE_HPP

#include<fstream>
#include <vector>
#include <string>
#include <map>


using namespace std;


string checkproperty ( string P) ;

string andfn (vector<string> args);

string orfn (vector<string> args);

string notfn (vector<string> args);

string iskeydef (vector<string> args) ;

string dictval (vector<string> args);

#endif
/***************************************************************************** 
****************************************************************************/