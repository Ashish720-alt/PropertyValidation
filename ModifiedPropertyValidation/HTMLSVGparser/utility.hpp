#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
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
#include<algorithm>
#include "macros.hpp"

using namespace std;

template <typename T> void printVectorOfVectors (vector<vector<T> > vec) {
    for (int i = 0; i < vec.size() ; i ++) {
        for (int j = 0; j < vec[i].size(); j++)
            cout << (vec[i])[j] << " ";
        cout << endl;
    }  
    return;  
}

template <typename T, typename U> void printVectorOfVectorofPairs (vector<vector<pair<T,U> > > vec) {
    for (int i = 0; i < vec.size() ; i ++) {
        for (int j = 0; j < vec[i].size(); j++)
            cout << (vec[i])[j].first << " " << (vec[i])[j].second << "    ";
        cout << endl;
    }  
    return;  
}


template <typename T, typename S> void printVectorOfMap (vector<map<T, S> > vmap) {
    for (long i = 0; i < vmap.size() ; i ++) {
        for (typename map<T,S>::iterator it=vmap[i].begin(); it!=vmap[i].end(); ++it)
            cout << it->first << ":" << it->second << endl;
    cout << ";" << endl;
    }
    return;
}

template <typename T, typename S> void printSetOfPairs (set<pair<T, S> > spair) {
    for (typename set<pair<T, S> >::iterator it = spair.begin(); it != spair.end(); ++it) {
        cout << it->first << " " << it->second << "\n";
    }
    return;
}

template <typename T, typename S> void printVectorOfPairs (vector<pair<T, S> > vpair) {
    for (int i = 0; i < vpair.size() ; i ++) {
        cout << vpair[i].first << " " << vpair[i].second << "\n";
    }
    return;
}


vector<string> split (string s, string delimiter);

pair<string,string> singleSplit (string s, string delimiter) ;

long stringToInt (string s); 

bool isStringNum (string s);

long getVectorIndex (vector <pair<long,long> > v, pair<long, long> node );

template <typename T> void printArrayofVectors (vector<T> av[NESTINGDEPTHMAX], long dep) {
    for (int i = 0; i < dep ; i ++) {
        for (int j = 0; j < av[i].size(); j++)
            cout << (av[i])[j] << " ";
        cout << endl;
    }  
    return;  
}

#endif