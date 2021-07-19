#include <vector>
#include <string>
#include <sstream> 
#include <utility>
#include <cstddef>  
#include "utility.hpp"

using namespace std;



vector<string> split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

pair<string,string> singleSplit (string s, string delimiter) {
    pair<string,string> temp;
    size_t index = s.find_first_of( delimiter );
    temp.first =  s.substr(0, index);
    temp.second = s.substr(index + 1);
    return temp;
}


long stringToInt (string s) {
    stringstream temp;
    long num;
    temp.str(s);
    temp >> num;
    return num;
}

bool isStringNum (string s) {
    string::iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

long getVectorIndex (vector <pair<long,long> > v, pair<long, long> node ) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i].first == node.first && v[i].second == node.second)
            return i;
    }
    return -1;
}