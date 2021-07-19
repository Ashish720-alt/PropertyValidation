#include<iostream>
#include<fstream>
#include <vector>
#include <string>
#include <sstream> 
#include <cstdlib>
#include <map> 
#include <iterator> 
#include <utility>
#include"../macros.h"
#include"../utility.h"



using namespace std;

// Check if you must replace int to long???
void readWSString (fstream& file, const string& s);
vector<vector<int> > readEdges (fstream& file, int N) ;
bool readUnlessDelim (fstream& file, string& line, const string& delim);


int main(int argc, char const *argv[])
{
    if ( argc != 2) {
        cerr << "Incorrect arguments: Give path to IR file!\n";
    }
    
    fstream readFile;
    readFile.open( argv[1], ios::in);
    string readLine;
    vector<string> lineList;
    int N, root;
    vector<vector<int> > treeEdges;
    vector<vector<int> > crossEdges;
    vector<map<string, string> > Data;
    map<string, string> nodeData;

    getline(readFile, readLine);
    lineList = split (readLine, " ");
    
    N = stringToInt ( lineList[0]);
    root = stringToInt ( lineList[1]);
    readWSString (readFile, "%");

    treeEdges = readEdges(readFile, N);

    readWSString (readFile, "%");

    crossEdges = readEdges(readFile, N);

    readWSString (readFile, "%");
    
    //printVectorOfVectors (treeEdges);
    //printVectorOfVectors (crossEdges);
    
    for (int i = 0; i < N ; i++ ) {
        nodeData.clear();
        while (readUnlessDelim(readFile, readLine, ";") ) {
            nodeData.insert(singleSplit(readLine, ":")) ;  
        }
        Data.push_back(nodeData);
    }

    // printVectorOfMap(Data);

    readWSString (readFile, "%");

    readFile.close();
    return 0;
}





void readWSString (fstream& file, const string& s) {
    string line;
    getline(file, line);
    if (line.compare(s)){
        cerr << "Incorrect syntax for IR file!\n";    
        abort();
    }
    return;
}


vector<vector<int> > readEdges (fstream& file, int N) {
    vector<int> tempEdges;
    string line;
    vector<string> lineList;
    vector<vector<int> > vec;
    
    for (int i = 0; i < N ; i ++) {
        getline(file, line);
        lineList = split (line, " ");
        if (lineList.size() == 1 && !lineList[0].compare("") ) { 
            vec.push_back( vector<int>() );
            continue;
        }    
        tempEdges.clear();
        for (int j = 0; j < lineList.size() ; j++) 
            tempEdges.push_back( stringToInt(lineList[j]) );
        vec.push_back(tempEdges) ;
    }
    return vec;
}

// Returns true iff line read isn't delim, also stores new value of line.
bool readUnlessDelim (fstream& file, string& line, const string& delim) {
    getline(file, line);
    return line.compare(delim);
}

