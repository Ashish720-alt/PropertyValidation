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
#include <algorithm>
#include <queue>
#include <ctype.h>
#include "../macros.hpp"
#include "../utility.hpp"
#include "../printFunctions.hpp"
#include "GraphPropertySpec.hpp"
#include "IRGraphTraversal.hpp"
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;

extern map<long,  map <string, string> > dictionaries;
extern vector< long > Sources;
extern vector< long > Leaves;
extern vector<long >  Graph; 
extern map<long, vector<long> > adjacencylist;
extern bool cycle ;
extern bool connected ;
extern bool orderedtree;
extern string graph_type;



map<long,  map <string, string> > getAllDictionaries (fstream& f, set<string> keys) {
    f.clear();                 
    f.seekg(0, ios::beg); 
    char id, id1, id2;
    string key;
    string value;
    map <string, string> d; 
    long ID;
    map<long,  map <string, string> > rv;

    id2 = charcompareabort( f, '[');
    while (id2 != ']') {
        id1 = charcompareabort( f, '{'); 
        while (id1 != '}') {  
            key = readstring(f);
            charcompareabort(f, ':');
            id = peekcharignoreWSEOFabort(f);
            if (id == '\"') {
                value = readstring(f);
            }
            else if (id == '[') {
                value = readValueArray(f);
            }
            else if (id == '{') {
                value = readValueDict(f);
            }
            else if ( isdigit(id) || id == '-' ) {
                value = readValueInt(f);
            }
            else {
                cerr << "Unexpected file format, expected either string, number, array or dictionary as value\n";
                abort();
            } 
            if ( keys.find( key ) != keys.end() ) {
                d.insert( pair<string, string> (key, value)  );
            }
            id1 = getcharignoreWSEOFabort(f);    
        }      
        ID = stringToInt(trimWS(d["\"Id\""]));
        rv.insert( pair<long, map<string, string> > (ID, d) ) ;
        d.clear(   );
        
        id2 = getcharignoreWSEOFabort(f);
    }

    return rv;
}

long N;

void PropertySpec ( fstream& specF, const string graphtype) {
    char c = peekcharignoreWSEOFabort( specF );
    while ( c != '-') {
        string P = readProperty(specF);  
        N = 0; // debug;
        switch ( propertyType(P, graphtype) ) {
            case 0:  // TreeRoot property
                
                // cout << "Source" << endl; // debug
                get_Sources ( P ) ;
                    

                break;

            case 1:  // Child property
                
                // cout << "Child" << endl;  // debug

                if (graphtype != "OT") 
                    childProperty ( P , graphtype) ;
                else 
                    orderedchildProperty ( P , graphtype) ;
                
                
                break;

            default:
                abort_with_error_message("Unexpected property type returned!\n");
                break;
        } 
        c = peekcharignoreWSEOFabort( specF );
    }
    charcompareabort(specF, '-');
    return;
}


void get_Sources ( string P ) {
    
    vector<string> property_split_at_where = split(P, "where" );
    for (long i = property_split_at_where.size() - 1; i > 0; i--) {
        bool unique = false;
        pair<string, string> nameprop = trimWS_pair( singleSplit(property_split_at_where[i], "satisfies") );
        if ( readwordfromstring( nameprop.first ) == "unique"  ) {
            unique = true;
            nameprop.first = trimWS( (singleSplit( nameprop.first, " " )).second) ;
        }
        vector< long  > traversalrv = finddictionary( nameprop.first, nameprop.second  ); 
        if (traversalrv.size() == 0 && unique) 
            abort_with_error_message("No dictionary satisfies \"" + property_split_at_where[i] + "\".\n") ;
        else if (traversalrv.size() > 1 && unique) 
            abort_with_error_message("Multiple dictionaries satisfy \""  + property_split_at_where[i] + "\".\n" );
        long ID = traversalrv[0]; 
        map<string, string> d = dictionaries[ID];
        for (long j = i - 1; j >= 0 ; j--) {
            property_split_at_where[j] = resolvesingledictref ( property_split_at_where[j], nameprop.first, d );   
        }
    }
    pair<string, string> property_split_at_satisfying = trimWS_pair( singleSplit(property_split_at_where[0], "satisfying") );
    pair<string, string> property_split_at_is = trimWS_pair( singleSplit( property_split_at_satisfying.first , "is" ) );
    bool uniquesrc = false;
    if (  readwordfromstring( trimWS(property_split_at_is.second) ) == "unique"  ) {
            uniquesrc = true;
            property_split_at_is.second = (singleSplit( trimWS(property_split_at_is.second), " " )).second ;
    }

    vector< long  > traversalrv = finddictionary( trimWS(property_split_at_is.second), trimWS(property_split_at_satisfying.second)  ); 
    if (traversalrv.size() == 0 && uniquesrc) 
        abort_with_error_message("No dictionary satisfies \"" + property_split_at_satisfying.second + "\".\n");
    else if (traversalrv.size() > 1 && uniquesrc) 
        abort_with_error_message("Multiple dictionaries satisfy \""  + property_split_at_satisfying.second + "\".\n");
    
    for (long i = 0; i < traversalrv.size() ; i++) {
        long ID = traversalrv[i]; 
        //map<string, string> d = dictionaries[ID];
        if (!is_element_in_Vector(Sources, ID) )
            Sources.push_back(ID) ;
        if (!is_element_in_Vector(Graph, ID))
            Graph.push_back( ID );
    }

    return;
}


void childProperty ( string P , const string graphtype) {
    pair<string, string> nameprop;
    pair<string, string> P1, P2, P3, P4, P5;
    map<string, string> d;
    queue< long>  Worklist;   
    set<long> IDsfound;
    long id, id2;
    vector<long> kids, kids2;
    bool unique = false;

    P1 = singleSplit(P, " where ");
    P2 = singleSplit(P1.first, ":"  );
    P3 = singleSplit(P2.first, " in ");
    if ( trimWS(P3.second) != graphtype ) 
        abort_with_error_message("Expected " + graphtype + " symbol here!\n"); 
    P4 = singleSplit( trimWS( P3.first ) , " "  );
    if ( trimWS(P4.first) != "forall" )
        abort_with_error_message("Expected 'forall' keyword here!\n");
    P5 = singleSplit(P2.second, " is ");
    if ((readwordfromstring(trimWS(P5.second)) == "unique")) {
        P5.second = (singleSplit( trimWS(P5.second), " ")).second;
        unique = true;
    }
    
    if (readwordfromstring(trimWS(P5.second)) != "Child") 
        abort_with_error_message("Expected 'Child' keyword here!\n");

    if ( trimWS(readstringbetween(P5.second, '(', ')')) != trimWS(P4.second) ) 
        abort_with_error_message("Quantifier symbol not same!\n");
    nameprop.first =  trimWS(P5.first);
    nameprop.second = trimWS(P1.second);
    Worklist = queue< long >();
    IDsfound.clear();
    for (long i = 0; i < Sources.size() ; i++) {
        long ID = Sources[i];
        d = dictionaries[ID];
        Worklist.push( ID );
        if ( IDsfound.find( ID ) != IDsfound.end() )
            abort_with_error_message("Multiple sources have the same ID.\n");
        IDsfound.insert(ID);
    }

    // auto graphcompute_start = high_resolution_clock::now();   

    while (!Worklist.empty() ) {
        kids.clear();
        nameprop.second = trimWS(P1.second);           
        
        id = Worklist.front();
        Worklist.pop();
        d = dictionaries[id];
        // id = stringToInt(trimWS(d["\"Id\""]));
        IDsfound.erase( id ); 
        nameprop.second = resolvesingledictref( trimWS(nameprop.second), trimWS(P4.second) , d);
        vector<long> traversalrv = finddictionary( trimWS(nameprop.first) , trimWS(nameprop.second)  );
        if (unique)
            if (traversalrv.size() != 1)
                abort_with_error_message("Expected unique child, but " + to_string(id) + " has " + to_string(traversalrv.size()) + " kids.\n");
        for (long i = 0; i < traversalrv.size(); i++) {
            d = dictionaries[ traversalrv[i] ];
            id2 = stringToInt(trimWS(d["\"Id\""]));
            if (id == id2) 
                abort_with_error_message("There is a self loop edge on " + to_string(id) + ".\n");
            if ( getVectorIndex(kids, id2) != -1) 
                abort_with_error_message("There are multiple edges from " + to_string(id) + " and " + to_string(id2) );
            kids.push_back(id2);  
            if (IDsfound.find( id2 ) == IDsfound.end() ) {
                Worklist.push( id2 );      
                IDsfound.insert(id2);
                Graph.push_back( id2 );
                continue;
            }
            else 
                continue;
        }
        if ( adjacencylist.find(id) == adjacencylist.end() ) {
            adjacencylist.insert( pair<long, vector<long> >(id, kids) );
        }
        else {
            kids2 = adjacencylist[id];
            kids.insert(kids.begin(), kids2.begin(), kids2.end());
            adjacencylist.insert( pair<long, vector<long> >(id, kids) );
        }
    }

    // auto graphcompute_stop = high_resolution_clock::now();
    // auto graphcompute_duration = duration_cast<microseconds>(graphcompute_stop - graphcompute_start);
    // float graphcompute_time_taken = ((float) graphcompute_duration.count()) / 1000000;
    // cout << "Worklist Time " << graphcompute_time_taken << endl;
    return;
}

vector< long > finddictionary (string name, string P) {
    vector< long > rv;
    // cout << N++ << endl; // Debug
    for (map<long,  map <string, string> >::iterator it = dictionaries.begin() ; it != dictionaries.end() ; ++it) {
        long ID = it->first;
        map <string, string> d = it->second;
        string Q = resolvesingledictref(P, name, d) ; 
        if ( Q != "" && checkproperty( Q) ) {
            rv.push_back(ID);
        }
    }

    return rv;
}

string resolvesingledictref (string P,  string name, map<string, string> d) { 
    if (name.empty()) {
        cerr << "Empty name string provided here!\n" << endl;
        abort();
    }
    string rv;
    long i;
    string field;
    string value;

    for (i = 0; i < P.length() ; i++) {
        if ((P[i] == name[0]) && (dictnameoccurrence(P, i, name, field) ) ) {
            if ( d.find(field) == d.end() )
                return "";
            value = d[field];
            rv.append(value);
            rv.push_back(P[i]);
            continue;
        }
        else {
            rv.push_back(P[i]);
        }
    }

    return rv;

}

bool dictnameoccurrence(string P, long& pos, string name, string& field  ) {
    long i = pos;
    long j;
    for (j = 0 ; j < name.size() ; i++, j++ ) {
        if (i == P.length() )
            return false;
        if (P[i] != name[j])
            return false;
    }
    if (i == P.length() || P[i] != '.') {
        return false;
    }
    i++;
    if (i == P.length() || P[i] != '<') {
        cerr << "Dictionary name, but not reference to a field\n";
        abort();
    }
    i++;
    field.clear();
    for ( ; i < P.length() && P[i] != '>'; i++) {
        field.push_back(P[i]);
    } 
    if (i == P.length()) {
        cerr << "Dictionary name, but final '>' is missing!\n";
        abort();        
    }
    i++;
    pos = i;
    field = trimWS(field); 
    return true;
}

string readProperty (fstream& f) {
    string rv;
    char c = getcharignoreWSEOFabort( f);
    while (  c != '.' || f.peek() != '.' ) {
        rv.append(1, c);
        c = charEOFabort(f);
    }
    charcompareabort(f, '.');
    return rv;
}

long propertyType( string P , const string graphtype) {
    string s = (singleSplit(P, " ")).first ; 
    if ( ((s.compare("Root") == 0) && (graphtype == "T" || graphtype == "F" || graphtype == "OT")) || 
            ((s.compare("Source") == 0) && (graphtype == "G")) )
        return 0;
    else if (s.compare("forall") == 0)
        return 1;
    else
    {
        cerr << "Incorrect Property format!\n";
        abort();
    }
    return -1;
     
}

void hascycleisconnected () {
    set<long> visited;
    set<long> rv;
    map<string, string> d;
    long id;
    bool t;
    long distinctsrc;

    cycle = false;
    for (long i = 0; i < Sources.size(); i++) {
        id = Sources[i];
        d = dictionaries[id];
        rv = BFS( id, t ) ;
        visited.insert(rv.begin(), rv.end() );
        cycle = cycle || t;
    }

    distinctsrc = 0;
    for (long i = 0; i < Sources.size(); i++) {
        id = Sources[i];
        d = dictionaries[id];
        if (id != -1 && visited.find( id ) == visited.end() )
            distinctsrc++;
        else if (id != -1 && visited.find( id ) != visited.end()) {
            cout << "Warning: Spec includes children nodes as sources!\n";
            Sources.erase(Sources.begin() + i );
            i--;
        }
    }

    connected = (distinctsrc <= 1);

}

set<long> BFS (long src, bool& singlesrccycle) {
    set<long> visited;
    long id;
    vector<long> kids;
    queue<long> Q;
    Q.push( src);
    visited.insert( src);
    singlesrccycle = false;
    while (!Q.empty() ) {
        id = Q.front();
        Q.pop();
        kids = adjacencylist[id];
        for (long j = 0; j < kids.size(); j++) {
            if (kids[j] == -1 ) // For ordered trees!
                continue;
            if ( visited.find(kids[j]) == visited.end()  ) {
                Q.push( kids[j] );
                visited.insert( kids[j]);
            }
            else {
                singlesrccycle = true;
            }
        }
    }
    visited.erase( src );
    return visited;
}

void orderedchildProperty ( string P , const string graphtype) {
    pair<string, string> nameprop;
    vector<string> Ps;
    pair<string, string> P1, P2, P3, P4, P5, P6;
    vector<long > traversalrv;
    map<string, string> d;
    queue< map <string, string> >  Worklist;   
    set<long> IDsfound;
    long id, id2;
    vector<long> kids, kids2;
    char childvar;
    string childvar2;
    string temp;
    string childprop;
    long childindex;

    Ps = split(P, "where");
    if (Ps.size() != 3) 
        abort_with_error_message("Incorrect format for property spec!\n");
    P1.first = Ps[0];
    P1.second = Ps[1];
    P2 = singleSplit(P1.first, ":"  );
    P3 = singleSplit(P2.first, " in ");
    if ( trimWS(P3.second) != graphtype ) 
        abort_with_error_message("Expected " + graphtype + " symbol here!\n");
    P4 = singleSplit( trimWS( P3.first ) , " "  );
    if ( trimWS(P4.first) != "forall" ) 
        abort_with_error_message("Expected 'forall' keyword here!\n");
    P5 = singleSplit(P2.second, " is ");
    temp = trimWS(P5.second);
    childvar = temp[0];
    temp.erase( temp.begin() );
    temp = (singleSplit( temp, "(" ) ).first;
    if (trimWS(temp) != "th Child") 
        abort_with_error_message("Expected \"<var_name>th Child\" keyword here!\n");
    if ( trimWS(readstringbetween(P5.second, '(', ')')) != trimWS(P4.second) ) 
        abort_with_error_message("Quantifier symbol not same!\n");
    P6 = singleSplit(Ps[2], " is ");
    childvar2 = trimWS(P6.first) ;
    if ( childvar2[0] != childvar) 
        abort_with_error_message("Quantifier to child number doesn't match\n");
    P6.second = trimWS(P6.second);

    nameprop.first =  trimWS(P5.first);
    nameprop.second = trimWS(P1.second);
    Worklist = queue< map <string, string> >();
    IDsfound.clear();
    for (long i = 0; i < Sources.size() ; i++) {
        id = Sources[i] ;
        d = dictionaries[id];
        Worklist.push( d );
        if ( IDsfound.find( id ) != IDsfound.end() ) 
            abort_with_error_message("Multiple sources have the same ID.\n");
        IDsfound.insert(id);
    }
    while (!Worklist.empty() ) {
        kids.clear();
        nameprop.second = trimWS(P1.second);           
        d = Worklist.front();
        Worklist.pop();
        id = stringToInt(trimWS(d["\"Id\""]));
        IDsfound.erase( id ); 
        nameprop.second = resolvesingledictref( trimWS(nameprop.second), trimWS(P4.second) , d);
        childprop = resolvesingledictref( trimWS(P6.second), trimWS(P4.second) , d);
        traversalrv = finddictionary( trimWS(nameprop.first) , trimWS(nameprop.second)  );
        for (long i = 0; i < traversalrv.size(); i++) {
            d = dictionaries[ traversalrv[i] ];
            childprop = resolvesingledictref( childprop , trimWS(nameprop.first), d);
            childindex = findChildIndex ( childprop);
            if (childindex == -1) 
                abort_with_error_message("Child Index is not found.\n");
            id2 = stringToInt(trimWS(d["\"Id\""]));
            if (id == id2) 
                abort_with_error_message("There is a self loop edge on " + to_string(id) + ".\n");
            if ( getVectorIndex(kids, id2) != -1) 
                abort_with_error_message("There are multiple edges from " + to_string(id) + " and " + to_string(id2));
            if (childindex < kids.size()) {
                kids[childindex - 1] = id2;
            }
            else {
                kids.resize( childindex, -1 );
                kids[childindex - 1] = id2;
            }
            if (IDsfound.find( id2 ) == IDsfound.end() ) {
                Worklist.push( d );      
                IDsfound.insert(id2);
                Graph.push_back( id2 );
                continue;
            }
            else {
                continue;
            }
        }
        if ( adjacencylist.find(id) == adjacencylist.end() ) {
            adjacencylist.insert( pair<long, vector<long> >(id, kids) );
        }
        else {
            kids2 = adjacencylist[id];
            kids.insert(kids.begin(), kids2.begin(), kids2.end());
            adjacencylist.insert( pair<long, vector<long> >(id, kids) );
        }
    }
    return;
}

void findleaves() {
    if (cycle == true || connected == false)
        abort_with_error_message("Cannot find leaves, not a tree.\n");
    vector<long> kids;
    long id;
    map <string, string> d;
    for (map<long, vector<long> >::iterator it = adjacencylist.begin(); it != adjacencylist.end(); ++it) {
        id = it->first;
        kids = it->second;
        if (kids.empty() ) 
            Leaves.push_back(id);
    }
    return;
}

long finddepth(long root_id) {
    if (cycle == true )
        abort_with_error_message("Cannot find tree depth, not acyclic.\n");
    
    queue<long> Q;
    Q.push(root_id);
    set<long> visited ;
    visited.insert(root_id);
    map<long , long> node_height_map ;
    node_height_map.insert(pair<long, long> (root_id, 0) );
    long height_max = 0;
    while (!Q.empty() ) {
        long id = Q.front();
        Q.pop();
        vector<long> kids = adjacencylist[id];
        long height = node_height_map[id];
        for (long j = 0; j < kids.size(); j++) {
            if (kids[j] == -1 ) // For ordered trees!
                continue;
            if ( visited.find(kids[j]) == visited.end()  ) {
                Q.push( kids[j] );
                visited.insert( kids[j]);
                node_height_map.insert( pair<long, long>(kids[j], height + 1) );
                height_max = max(height + 1, height_max);
            }
        }
    }

    return height_max;

}
