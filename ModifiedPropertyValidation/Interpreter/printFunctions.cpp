#include <iostream>
#include <map>
#include <string>
#include "macros.hpp"
#include "printFunctions.hpp"


using namespace std;

extern map<long,  map <string, string> > dictionaries;
extern vector< long > Sources;
extern vector< long > Leaves;
extern vector<long >  Graph; 
extern map<long, vector<long> > adjacencylist;
extern bool cycle ;
extern bool connected ;
extern bool orderedtree;
extern string graph_type;


void printgraph(bool leaves) {
    cout << "Graph node count: " << Graph.size() << endl << endl;
    cout << "All graph nodes: \n";
    printVector(Graph);
    cout << "\nAll source nodes:\n";
    printVector(Sources);
    if (leaves) {
        cout << "\nAll leaf nodes:\n";
        printVector(Leaves);
    }
    cout << "\nThe adjacency list of the graph is:\n";
    map<long, vector<long> >::iterator it2;
    for (it2 = adjacencylist.begin(); it2 != adjacencylist.end(); it2++) {
        cout << it2->first << ": ";
        printVector(it2->second);
    }
    cout << endl;
}

void printImpKeys(set<string> IK) {
    cout << "Keys used in spec: ";
    printSet (IK);
    cout << endl ; 
}