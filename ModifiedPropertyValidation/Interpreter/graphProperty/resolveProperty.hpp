#ifndef RESOLVEPROPERTY_HPP
#define RESOLVEPROPERTY_HPP

#include<fstream>
#include <vector>
#include <string>
#include <map>


using namespace std;


bool graph_checkproperty ( string P) ;

long graph_findChildIndex ( string P);

long graph_indexnfn (vector<string> args);

long graph_indexsfn (vector<string> args);

string resolve_node_reference (struct property_data PD, map<string, string> d);

bool have_reference (string P, long& pos, string name, string& key  );

string resolve_retvalFunction_reference (struct property_data PD);

bool have_retvalFunction_reference (string p, long& pos, string function_name, string& argstring ) ;

#endif

/****************************************************************************
Function and Return Value: 
****************************************************************************/