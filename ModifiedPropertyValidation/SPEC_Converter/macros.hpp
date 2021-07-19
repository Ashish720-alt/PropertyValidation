#ifndef MACROS_HPP
#define MACROS_HPP

#include<map>
#include <vector>
#include <string>

using namespace std;

#define UNCHECKED -1
#define TRUE 1
#define FALSE 0


extern map<long,  map <string, string> > dictionaries;
extern vector< long > Sources;
extern vector< long > Leaves;
extern vector<long >  Graph; 
extern map<long, vector<long> > adjacencylist;
extern bool cycle ;
extern bool connected ;
extern bool orderedtree;
extern string graph_type;
extern long graph_depth;




#endif

