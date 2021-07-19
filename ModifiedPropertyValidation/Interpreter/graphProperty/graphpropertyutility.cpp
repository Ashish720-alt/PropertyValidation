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
#include <ctype.h>
#include "../macros.hpp"
#include "../utility.hpp"
#include "resolveProperty.hpp"
#include "graphmacros.hpp"


using namespace std;

bool is_PATH_reference(string P) {
   string word= readwordfromstring(P);
   return (word == "PATH") ;
}

vector<long> BFS_nodes_visited (long source, map<long, vector<long> > adj_list) {
    set<long> visited_nodes;
    queue<long> Q;

    Q.push(source);

    while (!Q.empty()) {
        long new_node = Q.front();
        Q.pop();
        visited_nodes.insert(new_node);
        vector<long> neighbours = adj_list[new_node] ;
        for (vector<long>::iterator it = neighbours.begin() ; it != neighbours.end(); ++it) {
            if ( visited_nodes.find(*it) == visited_nodes.end() )
                Q.push( *it );
        }
    }

    vector<long> retval (visited_nodes.begin(), visited_nodes.end());
    return retval;
}


map<long, vector<long> > set_of_edges_to_adjacency_list_representation( vector<pair<long, long> > edges  ) {
    map<long, vector<long> > adj_list;
    for (long i = 0; i < edges.size(); i++) {
        long hd_id = edges[i].first;
        long tl_id = edges[i].second;
        vector<long> kids;
        if ( adj_list.find(hd_id) != adj_list.end() ) {
            kids = adj_list[hd_id];
            adj_list.erase(hd_id);
        }
        kids.push_back(tl_id);
        adj_list.insert( pair<long, vector<long> >(hd_id, kids) );
    }
    return adj_list;
}

bool SC_have_reference (string p, long& pos, string name, string& key  ) {
    long i = pos;
    for (long j = 0 ; j < name.size() ; i++, j++ ) {
        if (i == p.length() )
            return false;
        if (p[i] != name[j])
            return false;
    }
    if (i == p.length() || p[i] != '.') {
        return false;
    }
    i++;
    if (i == p.length() || p[i] != '<') 
        abort_with_error_message("Dictionary name, but not reference to a key\n");
    i++;
    key.clear();
    for ( ; i < p.length() && p[i] != '>'; i++) {
        key.push_back(p[i]);
    }
    if (i == p.length()) 
        abort_with_error_message("Dictionary name, but final '>' is missing!\n");
    pos = i;
    key = trimWS(key); 
    return true;
}

void print_property_evaluation (string P, bool is_property_true) {
    cout << "\"" << P  << "\" " ;
    if (is_property_true)
        cout << "holds true for this file.\n\n";
    else
        cout << "does NOT hold true for this file.\n\n";
}

bool have_reference (string p, long& pos, string name, string& key  ) {
    long i = pos;
    for (long j = 0 ; j < name.size() ; i++, j++ ) {
        if (i == p.length() )
            return false;
        if (p[i] != name[j])
            return false;
    }
    if (i == p.length() || p[i] != '.') {
        return false;
    }
    i++;
    if (i == p.length() || p[i] != '<') 
        abort_with_error_message("Dictionary name, but not reference to a key\n");
    i++;
    key.clear();
    for ( ; i < p.length() && p[i] != '>'; i++) {
        key.push_back(p[i]);
    }
    if (i == p.length()) 
        abort_with_error_message("Dictionary name, but final '>' is missing!\n");
    pos = i;
    key = trimWS(key); 
    return true;
}


string resolve_retvalFunction_reference (struct property_data PD) {
    string var_name = PD.variable_name;
    string property = PD.property;
    if (var_name.empty()) 
        abort_with_error_message("Empty name string provided here!\n");
    
    string property_without_references;
    string value;
    for (long i = 0; i < property.length() ; i++) {
        if ( have_retvalFunction_reference(property, i, "REFSTRING", value) ) {
            value = trimWS(value);
            if (is_enclosed_by_delimiters(value, '\"', '\"' ) )
                value.insert(1, "#" );
            property_without_references.append( value );
            continue;
        }
        else 
            property_without_references.push_back(property[i]);
    }

    return property_without_references;
}    


bool have_retvalFunction_reference (string p, long& pos, string function_name, string& argstring ) {
    long i = pos;
    for (long j = 0 ; j < function_name.size() ; i++, j++ ) {
        if (i == p.length() || p[i] != function_name[j])
            return false;
    }
    
    while ( isWS(p[i]) ) {
        if (i == p.length() - 1)
            abort_with_error_message("String ended without arguments");
        i++;
    }

    if (p[i] != '(')
        abort_with_error_message("Expected '(' in retvalFunction instead of " + to_string(p[i]) );
    argstring = readstringbetween( p.substr(i, p.size() - i ) , '(', ')') ;

    i++;
    long cb = 0;
    long sb = 0;
    long ob = 1;
    for( ; (p[i] != ')') || (cb != 0 || sb != 0 || ob != 1) ; i++ ) {
        if (i == p.length() - 1) 
            abort_with_error_message("p has '(' but no occurrence of ')'.\n");
        if (p[i] == '[')
            sb ++;
        else if (p[i] == ']')
            sb--;
        else if (p[i] == '{')
            cb++;
        else if (p[i] == '}')
            cb--;
        else if (p[i] == '(')
            ob++;
        else if (p[i] == ')')
            ob--;
    }    

    pos = i;
    return true;
}