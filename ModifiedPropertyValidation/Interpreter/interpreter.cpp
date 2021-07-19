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
#include "./getGraph/IRGraphTraversal.hpp"
#include "./getGraph/Keys.hpp"
#include "./graphProperty/graphPropertySpec.hpp"
#include "./graphProperty/graphmacros.hpp"
#include "./graphProperty/resolveProperty.hpp"
#include "./AG/AGspec.hpp"
#include <algorithm>
#include <chrono>
// #include "./SemanticCondition/AG_resolveSC1.hpp"
// #include "./SemanticCondition/AG_resolveSC_functions1.hpp"
// #include "./SemanticCondition/AGutility1.hpp"
#include "./SemanticCondition/graphmacros1.hpp"  
#include "./SemanticCondition/graphpropertyutility1.hpp" 
#include "./SemanticCondition/resolveSemanticCondition1.hpp"  
#include "./SemanticCondition/resolveSemanticCondition_functions1.hpp"     
#include "./AG/AGutility.hpp"

using namespace std;
using namespace std::chrono;

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


/* 
    Map sorts elements based on key. Implementation always assumes that there is key named "Id" of type 'long', on whose basis
    dictionaries are stored and referred. 
*/

int main(int argc, char const *argv[])
{    
    if ( argc != 3) 
        abort_with_error_message("Incorrect arguments: Give path to .json file and Spec file!\n");

    fstream JSONFile;
    fstream specFile;
    set<string> impKeys;
    char c;
    bool eof;
    
    // Open .json file.
    JSONFile.open( argv[1], ios::in);
    
    // Convert surface Spec to Internal Spec file.
    string internalspec_filename = surface_to_internal_spec( argv[2] );
    specFile.open( internalspec_filename.c_str(), ios::in);

    // Read all dictionaries (with necessary keys only) from .json file
    impKeys = keylist ( specFile );

  

    dictionaries = getAllDictionaries(JSONFile, impKeys );
    
    //cout << "HOO" << endl; // Debug!
    /*  // Print all important keys, and all dictionaries:
        printImpKeys( impKeys);
        print_Map_of_ID_and_Maps(dictionaries);
    */  

     

    //cout << dictionaries.size() << endl;

    

    // Get graph structure.
    auto graphcompute_start = high_resolution_clock::now();
    specFile.clear();                 
    specFile.seekg(0, ios::beg);    
    c = peekcharignoreWSEOFabort( specFile);
    switch (c) {
        case 'T': // Tree
            stringcompareabort(specFile, "T:" );
            graph_type = "T";
            PropertySpec(specFile,  "T" );
            hascycleisconnected ();
            if (cycle == true || connected == false) 
                abort_with_error_message("Graph is not a tree.\n");
            orderedtree = false;
            break;
        
        case 'F': // Forest
            stringcompareabort(specFile, "F:" );
            graph_type = "F";
            PropertySpec(specFile,  "F" );
            hascycleisconnected ();
            if (cycle == true) 
                abort_with_error_message("Graph is not a forest.\n");
            orderedtree = false;
            break;
        
        case 'G': // General graph
            stringcompareabort(specFile, "G:" );
            graph_type = "G";
            PropertySpec(specFile,  "G" );
            hascycleisconnected ();
            orderedtree = false;
            break;
        
        case 'O': // Ordered Tree
            stringcompareabort(specFile, "OT:" );
            graph_type = "OT";
            PropertySpec(specFile,  "OT" );
            hascycleisconnected ();
            if (cycle == true || connected == false) 
                abort_with_error_message("Graph is not a tree.\n");
            orderedtree = true;
            break;
        
        default: // Error
            abort_with_error_message("Unexpected file format, expected T, F, G or OT here!\n");
    }


    // Find additional graph data - Leaves and graph_depth.
    if (cycle == false && connected == true) {
        findleaves();
        graph_depth = finddepth( Sources[0] );
    }

    // Print ALL graph information.
    printgraph( (cycle == false && connected == true) );

    // // Get time to compute graph.
    auto graphcompute_stop = high_resolution_clock::now();
    auto graphcompute_duration = duration_cast<microseconds>(graphcompute_stop - graphcompute_start);
    float graphcompute_time_taken = ((float) graphcompute_duration.count()) / 1000000;

    // Solve attributes, graph specification or ordered tree specification.
    auto propertyresolve_start = high_resolution_clock::now();
    eof = false;
    
    
    string word = peekwordignoreWSEOFabort(specFile);
    stringcompareabort(specFile, "SD:");
    word = peekwordignoreWSEOFabort(specFile);
    while (word != "%") {
        if (word == "forall" || word == "exists") // Attributes.
            AGSpec( specFile );
        else
            graph_property_specification(specFile); // Graph Specification.
        
        word = peekwordignoreWSEOFabort(specFile);
    }
    print_all_attributes ();
    cout << "Node Sets are: " << endl;
    printMapof_T_and_vector_of_S( name_node_map );
    cout << endl;
    cout << "Edge Sets are: " << endl;
    printMapof_T_and_vector_of_pair_of_UV( name_edge_map);
    cout << endl;

    stringcompareabort(specFile, "%");

    // Check for graphs!
    char nextchar = peekcharignoreWSEOFabort( specFile );
    while ( nextchar != SPECIFICATION_DELIMITER) {
        string condition_spec = read_SC_spec_without_delimiters_and_abort_if_wrong_format(specFile);
        graphSC_check_HigherOrderLogic_property(condition_spec) ; 
        nextchar = peekcharignoreWSEOFabort( specFile );
    }    
    charcompareabort(specFile, SPECIFICATION_DELIMITER);



    // Close .json file and internal spec file.
    JSONFile.close();
    specFile.close();

    // Get time to compute graph.
    auto propertyresolve_stop = high_resolution_clock::now();
    auto propertyresolve_duration = duration_cast<microseconds>(propertyresolve_stop - propertyresolve_start);
    float propertyresolve_time_taken = ((float) propertyresolve_duration.count()) / 1000000;

    // Time taken to find graph and solve properties.
    cout << "Time taken to compute graph: " << graphcompute_time_taken << " s." << endl;
    cout << "Time to resolve all properties: " << propertyresolve_time_taken << " s." << endl << endl;
    
    // Delete Internal Spec file
    remove( internalspec_filename.c_str() ); 
    return 0;
}
