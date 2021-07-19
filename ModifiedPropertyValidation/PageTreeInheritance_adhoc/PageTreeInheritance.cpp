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
#include "macros.hpp"
#include "utility.hpp"
#include <time.h>

using namespace std;

string readtext (fstream& file) ;
pair<long,long> objectAnalyzer(fstream& file, bool& page, vector<pair<long,long> >& nodeedge, pair<bool,bool>& attr) ;
bool BFS(vector <pair<long,long> > idmap, vector <vector<pair<long,long> > > edgemap, 
                                vector<pair<bool, bool> >& defattr, pair<long, long> root);


int main(int argc, char const *argv[])
{
    clock_t tStart = clock();
    if ( argc != 2) {
        cerr << "Incorrect arguments: Give path to IR file!\n";
    }
    
    fstream readFile;
    readFile.open( argv[1], ios::in);
    string text;
    long N = 0;
    long cb = 0;
    long sb = 0;
    pair<long, long> id;
    vector <pair<long,long> > idmap;
    vector <vector<pair<long,long> > > edgemap;
    vector<pair<long,long> > nodeedge;
    vector<pair<bool, bool> > defattr;
    pair<bool,bool> attrtemp;
    pair<long, long> root;
    pair<long, long> root2 ;
    bool page = false;
    bool condition;
    
    text = readtext(readFile);
        if (text.compare("[") != 0) {
            cerr << "Unexpected file format, expected '[' as first symbol\n";
        }
    sb = 1;

    while (sb != 0 || cb != 0) {
        text = readtext(readFile);
        if (text.compare("[") == 0)
            sb++;
        else if (text.compare("]") == 0)
            sb--;
        else if (text.compare("{") == 0)
            cb++;
        else if (text.compare("}") == 0) {
            cb--;
            if (cb == 0) {
                if (page) {
                    N++;
                    idmap.insert(idmap.end(), id);
                }
                page = false;
                nodeedge.clear();
            }
                
        }
        else if (sb == 1 && cb == 1 && text.compare("\"obj\"") == 0) {
            text = readtext(readFile);
            id.first = atol(text.c_str());    
            text = readtext(readFile);
            if (text.compare("\"gen\"") != 0) {
                cerr << "Unexpected file format, expected 'gen' after 'obj'\n";
            }
            text = readtext(readFile);
            id.second = atol(text.c_str());
            
        }
        else if(text.compare("\"value\"") == 0) {
            attrtemp.first = false;
            attrtemp.second = false;
            root = objectAnalyzer( readFile, page, nodeedge, attrtemp);
            if (page) {
                edgemap.insert( edgemap.end(), nodeedge); 
                defattr.insert (defattr.end(), attrtemp);
            }
            if (root.first != -1 || root.second != -1)  
                root2 = root;
        }

    }
    readFile.close();

    // // Debugging purposes only!    
    // cout << N << endl << endl;
    // printVectorOfPairs(idmap);
    // cout << "\n";
    // cout << root2.first << ' ' << root2.second << ' ' << '\n';
    // cout << "\n";
    // printVectorOfVectorofPairs(edgemap);
    // cout << "\n";
    // printVectorOfPairs(defattr);
    // cout << endl;


    condition = BFS(idmap, edgemap, defattr, root2 );

    // // Debugging purposes only!
    // printVectorOfPairs(defattr);
    // cout << endl;

    if (condition)
        cout << "Property valid." << endl;
    else
        cout << "Property Invalid." << endl;

    
    cout << (double)(clock() - tStart)/CLOCKS_PER_SEC << endl ;
    return 0;

}


string readtext (fstream& file) {
    string str = "";
    char buff;
    
    buff = ' ';
    while( buff == ' ' || buff == '\n' || buff == ',' || buff == ':')
        file.get( buff) ;
    while (buff != ' ' && buff != '\n' && buff != ',' && buff != ':') {
        str.append(1, buff);
        file.get(buff);
    }    

    return str;
}

pair<long,long> objectAnalyzer(fstream& file, bool& page, vector<pair<long,long> >& nodeedge, pair<bool,bool>& attr) {
    long sb2 = 0;
    long cb2 = 0;
    string text;
    pair<long, long> retval (-1,-1);
    bool catalog = false;
    bool ref = false;
    pair<long, long> edgenode (-1,-1) ;

    text = readtext(file);
        if ( isStringNum (text)  )
            return pair<long,long> (-1,-1);
        if (text.compare("{") == 0) 
            cb2 = 1;    
        else if(text.compare("[") == 0)
            sb2 = 1;
        else                // is a string!
            return pair<long,long> (-1,-1);
    
    while (sb2 != 0 || cb2 != 0) {
        text = readtext(file);
        if (text.compare("[") == 0)
            sb2++;
        else if (text.compare("]") == 0)
            sb2--;
        else if (text.compare("{") == 0)
            cb2++;
        else if (text.compare("}") == 0)
            cb2--;
        else if (cb2 == 2 && sb2 == 0 && text.compare("\"Type\"") == 0) {
            text = readtext(file);
            if (text.compare("{") != 0) 
                continue;
            cb2++;
            text = readtext(file);
            if (text.compare("\"$name\"") != 0)
                continue ; 
            text = readtext(file);
            if (text.compare("\"Catalog\"") == 0) {       
                text = readtext(file);
                if (text.compare("}") != 0) 
                    continue;            
                cb2-- ;
                catalog = true;     
            }
            else if (text.compare("\"Page\"") == 0 || text.compare("\"Pages\"") == 0) {
                page = true;
                continue;
            }
            else
                continue;
        }
        else if (cb2 == 2 && sb2 == 0 && text.compare("\"Pages\"") == 0) {
            text = readtext(file);
            if (text.compare("{") != 0)
                continue ;
            cb2++;
            text = readtext(file);
            if (text.compare("\"$ref\"") != 0)
                continue ;  
            text = readtext(file);
            if (text.compare("{") != 0)
                continue;
            cb2++;
            text = readtext(file);
            if (text.compare("\"obj\"") != 0)
                continue ;    
            text = readtext(file);
            retval.first = atol(text.c_str());  
            text = readtext(file);
            if (text.compare("\"gen\"") != 0)
                continue ;  
            text = readtext(file);
            retval.second = atol(text.c_str());   
            text = readtext(file);
            if (text.compare("}") != 0)
                continue;
            cb2--;
            text = readtext(file);   
            if (text.compare("}") != 0)
                continue;
            cb2--;
            ref = true;            
        }
        else if (cb2 == 2 && sb2 == 0 && text.compare("\"Kids\"") == 0) {
            text = readtext(file);
            if (text.compare("[") != 0)
                continue ;
            sb2++;
            text = readtext(file);
            nodeedge.clear();
            while (text.compare("]") != 0) {
                cb2++;
                text = readtext(file);
                text = readtext(file);
                cb2++;
                text = readtext(file);
                text = readtext(file);
                edgenode.first = atol(text.c_str());
                text = readtext(file);
                text = readtext(file);
                edgenode.second = atol(text.c_str());   
                text = readtext(file);
                cb2--;
                text = readtext(file);
                cb2--;  
                nodeedge.insert(nodeedge.end() , edgenode); 
                text = readtext(file);                         
            }
            sb2--;
            continue;
        }
        else if (cb2 == 2 && sb2 == 0 && text.compare("\"Resources\"") == 0) {
            attr.first = true;
            continue;        
        }
        else if (cb2 == 2 && sb2 == 0 && text.compare("\"MediaBox\"") == 0) {
            attr.second = true;
            continue;        
        }
    }
    
    if (ref && catalog)
        return retval;
    

    return pair<long,long> (-1,-1);
}

bool BFS(vector <pair<long,long> > idmap, vector <vector<pair<long,long> > > edgemap, 
                                vector<pair<bool, bool> >& defattr, pair<long, long> root) {
    queue<long> Q;
    Q.push( getVectorIndex(idmap, root) );
    while ( !Q.empty() ) {
        long i = Q.front();
        Q.pop();
        if ( edgemap[i].empty() ) {
            if ( defattr[i].first != true || defattr[i].second != true )
                return false; 
        }

        for (long j = 0; j < edgemap[i].size(); j++) {
            long k =  getVectorIndex(idmap, (edgemap[i])[j]) ;
            defattr[k].first = defattr[k].first || defattr[i].first;
            defattr[k].second = defattr[k].second || defattr[i].second;
            Q.push(k);
        }
    }


    return true;
}



