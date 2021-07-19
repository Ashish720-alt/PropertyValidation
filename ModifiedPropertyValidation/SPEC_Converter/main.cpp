#include<iostream>
#include<fstream>
#include <vector>
#include <string>
#include <sstream> 
#include <cstdlib>
#include <map> 
#include <set>
#include <iterator> 
#include <utility>
#include <queue>
#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include "macros.hpp"
#include "utility.hpp"
#include "printFunctions.hpp"
#include "surface_to_internal.hpp"

using namespace std;

map<long,  map <string, string> > dictionaries;
vector< long > Sources;
vector< long > Leaves;
vector<long >  Graph; 
map<long, vector<long> > adjacencylist;
bool cycle ;
bool connected ;
bool orderedtree;
string graph_type;
long graph_depth;

int main(int argc, char const *argv[])
{    
    if ( argc != 2) {
        cerr << "Incorrect arguments: Give path to Spec file!\n";
        abort();
    }


    string internalspec_filename = surface_to_internal_spec( argv[1] );

    return 0;
}