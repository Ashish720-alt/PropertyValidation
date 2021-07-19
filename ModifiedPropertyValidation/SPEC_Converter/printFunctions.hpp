#ifndef PRINTFUNCTIONS_HPP
#define PRINTFUNCTIONS_HPP

#include<iostream>
#include <vector>
#include <string>
#include <map> 
#include <set>
#include <iterator> 
#include <utility>


using namespace std;


template <typename T> void printVectorOfVectors (vector<vector<T> > vec) {
    for (long i = 0; i < vec.size() ; i ++) {
        for (long j = 0; j < vec[i].size(); j++)
            cout << (vec[i])[j] << " ";
        cout << endl;
    }  
    return;  
}

template <typename T, typename U> void printVectorOfVectorofPairs (vector<vector<pair<T,U> > > vec) {
    for (long i = 0; i < vec.size() ; i ++) {
        for (long j = 0; j < vec[i].size(); j++)
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
    for (long i = 0; i < vpair.size() ; i ++) {
        cout << vpair[i].first << " " << vpair[i].second << "\n";
    }
    return;
}

template <typename T, typename S> void printMap (map<T, S>  map) {
    cout << "{\n";
    for (typename map<T,S>::iterator it=map.begin(); it!=map.end(); ++it)
        cout << "  " << it->first << ": " << it->second << endl;
    cout << "}\n" ;
    return;
}

template <typename T> void printSet ( set<T>  s) {
    long i = 0;
    cout << "[ ";
    for ( typename set<T>::iterator it=s.begin(); it!=s.end(); i++, ++it) {
        cout << *it << " ";
        if( i != (s.size() - 1) )
            cout << ", " ;
    }
    cout << "]\n" ;
    return;
}

template <typename T> void printVector ( vector<T>  v) {
    cout << "[ ";
    for (typename vector<T>::iterator it=v.begin(); it!=v.end(); ++it) {
        cout << *it << " ";
        if (it != v.end() - 1)
            cout << ", ";    
    }    
    cout << "]\n" ;
    return;
}

template <typename T, typename S> void printMapof_T_and_vector_of_S ( map<T, vector<S> > m_v ) {
    cout << "{\n";
    for (typename map<T, vector<S> >::iterator it=m_v.begin(); it!=m_v.end(); ++it) {
        cout << "  " << it->first << ": " ;
        vector<S> v = it->second;
        cout << "[ ";
        for (typename vector<S>::iterator it2 = v.begin() ; it2 != v.end(); ++it2 ) {
            cout << *it2 << " ";    
        }
        cout << "] " << endl;
    }
    cout << "}\n" ;
    return;
}   


template <typename T, typename U, typename V> void print_Map_of_ID_and_Maps ( map<T, map<U,V> > m_m ) {
    for (typename map<T,  map <U, V> >::iterator it = m_m.begin(); it != m_m.end(); it++) {
        printMap(it->second);
    }
    return;
}



template <typename T, typename U, typename V> void printMapof_T_and_vector_of_pair_of_UV 
    ( map<T, vector< pair<U, V> > > m_vp ) {
    cout << "{\n";
    for (typename map<T, vector< pair<U, V> > >::iterator it=m_vp.begin(); it!=m_vp.end(); ++it) {
        cout << "  " << it->first << ": " ;
        vector< pair<U,V> > v = it->second;
        cout << "[ ";
        for (typename vector< pair<U,V> >::iterator it2 = v.begin() ; it2 != v.end(); ++it2 ) {
            pair<U,V> p = *it2;
            cout << "( " << p.first << "," << p.second << " )";    
        }
        cout << "] " << endl;
    }
    cout << "}\n" ;
    return;
}  




void printgraph(bool leaves);

void printImpKeys(set<string> IK);

#endif
