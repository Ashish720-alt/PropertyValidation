#ifndef GRAPHSPEC_HPP
#define GRAPHSPEC_HPP

#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <set>
#include <string>
#include "../macros.hpp"

using namespace std;



map<long,  map <string, string> > getAllDictionaries (fstream& f, set<string> keys);

void PropertySpec ( fstream& specF, const string graphtype) ;


void get_Sources ( string P ) ;


void childProperty ( string P , const string graphtype);


vector< long > finddictionary (string name, string P) ;


string resolvesingledictref (string P,  string name, map<string, string> d) ;


bool dictnameoccurrence(string P, long& pos, string name, string& field  );


string readProperty (fstream& f) ;


long propertyType( string P, const string graphtype ) ;


void hascycleisconnected () ;


set<long> BFS (long src, bool& singlesrccycle);


void orderedchildProperty ( string P , const string graphtype);

void findleaves();

long finddepth(long root_id);

#endif

