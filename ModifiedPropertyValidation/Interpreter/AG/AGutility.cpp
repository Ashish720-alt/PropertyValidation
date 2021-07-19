#include<iostream>
#include<set>
#include<vector>
#include<map>
#include<string>
#include<fstream>
#include<stdlib.h>
#include "AGspec.hpp"
#include "AGmacros.hpp"
#include "AGutility.hpp"
#include "../macros.hpp"
#include "../utility.hpp"
#include "../printFunctions.hpp"

using namespace std;

vector<long> resolve_CHILD_reference (string P) {
    long ref_position = P.find("Child", 0);
    if (ref_position != 0)
        abort_with_error_message(P + "is not a Child reference!\n");
    long arg_start_position = P.find("(", ref_position  );
    if (arg_start_position == string::npos)
        abort_with_error_message("No arguments to Child reference!");
    long arg_end_position = P.find(")", arg_start_position );  
    if (arg_end_position == string::npos)
        abort_with_error_message("No terminating bracket to arguments of Child reference!");
    
    vector<string> arguments =  split( P.substr(arg_start_position + 1, arg_end_position - arg_start_position - 1), ","  ) ;
    if (arguments.size() != 1)
        abort_with_error_message("Incorrect number of arguments to Child reference!\n");
    
    long start_node_id = stringToInt(arguments[0]);

    return adjacencylist[ start_node_id];    
}

bool is_CHILD_reference(string P) {
   string word= readwordfromstring(P);
   return (word == "Child") ;
}

pair<string, string> AG_get_dictName_and_dictArray (string property) {
    pair<string, string> dictName_and_dictArray = trimWS_pair(singleSplit( property, "in" ));
    string dictName = trimWS((singleSplit(dictName_and_dictArray.first, " ")).second);
    if ((singleSplit(dictName_and_dictArray.first, " ")).first != "forall")
        abort_with_error_message("Expected 'forall' keyword as quantifier first word!\n");
    dictName_and_dictArray.first = dictName;
    return dictName_and_dictArray;
}

string AG_resolve_references (string P, map<string,long > quantifier_map) {
    map<string, string> dict;
    string property = P;
    for (map<string,long >::iterator it = quantifier_map.begin(); it != quantifier_map.end(); ++it) {
        dict = dictionaries[ it->second ];
        property = AG_resolve_single_reference(it->first, property, dict);
    }
    return property;
}

string AG_resolve_single_reference (string var_name, string property, map<string, string> d) { 
    if (var_name.empty()) 
        abort_with_error_message("Empty name string provided here!\n");
    
    string property_without_references;
    string key;
    string value;
    for (long i = 0; i < property.length() ; i++) {
        if ( AG_have_reference(property, i, var_name, key) ) {
            value = ( d.find(key) == d.end() ) ? "" : d[key];
            property_without_references.append(value);
            continue;
        }
        else 
            property_without_references.push_back(property[i]);
    }

    return property_without_references;
}

bool AG_have_reference (string p, long& pos, string name, string& key  ) {
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

string read_AG_property_without_delimiters (fstream& f) {
    string property;
    char c = getcharignoreWSEOFabort( f);
    while (  c != PROPERTY_END_DELIMITING_CHARACTAR || f.peek() != PROPERTY_END_DELIMITING_CHARACTAR ) {
        property.append(1, c);
        c = charEOFabort(f);
    }
    charcompareabort(f, PROPERTY_END_DELIMITING_CHARACTAR);
    return property;
}

void print_prop (string P, bool propval) {
    cout << "The AG property \"" + P + "\"" ;
    if (propval)
        cout << " holds true for this file!\n\n";
    else
        cout << " does not hold true for this file!\n\n";
    return;
}



void print_all_attributes () {

    long id; 
    map <string, string> dict;
    string attribute;
    cout << "All attribute values: \n";
    for (map<long,  map <string, string> >::iterator it = dictionaries.begin(); it != dictionaries.end() ; ++it ) {
        id = it->first;
        dict = it->second;
        cout << id << ": \n  {\n";
        for (map<string, string>::iterator it2 = dict.begin(); it2 != dict.end() ; ++it2) {
            attribute = it2->first;
            if (!is_enclosed_by_delimiters(attribute, '\"', '\"') )
                cout << "    " << attribute << " : " << it2->second << endl;
        }
        cout << "  }" << endl << endl; 
    }
}