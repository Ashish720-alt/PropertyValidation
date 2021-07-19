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
#include<algorithm>
#include "macros.hpp"
#include "utility.hpp"


using namespace std;

long ct = 1;
long depth = 0;


string readTag (fstream& file, bool& emptyval, bool& EOFread) ;
char readchar (fstream& file, bool& EOFread);
string readtilldelim (fstream& file, const char delim, bool& EOFread);
void printNodes (vector<struct node*> nodes, string filename);
bool isNewlineString (string s);
void changecharinString (string& s, char init, char final);
bool isWSstring (string s);
string trimWS (string s) ;
bool isWS (char c);

struct node {
    long id;
    string name;
    vector<long> kids;
    string value;
    string Content;

    node() {
        kids.clear();
        Content = "";
    }
};

/* Child of a tags: Any tag after an open tag (tag without end tag) is considered a child of the open tag
until you reach the end tag of the parent tag of the open tag.*/


int main(int argc, char const *argv[])
{
    
    if ( argc != 2) {
        cerr << "Incorrect arguments: Give path to IR file!\n";
    }
    
    fstream readFile;
    
    readFile.open( argv[1], ios::in);
    string word;
    char c;
    vector <struct node* > nodes;
    vector<long> depthtable[NESTINGDEPTHMAX] ;
    struct node* temp;
    string val;
    bool tagattr = false;
    long nodeid;
    string st;
    bool selfclosing;
    bool EOFread = false;
    string str;

   // long t = 1;
    
    while (readFile.peek() != EOF ) {
       // t++;
        st = readtilldelim(readFile, '<', EOFread);
        if (EOFread)
            break;

        st.erase(remove(st.begin(), st.end(), '\r'), st.end());
        st.erase(remove(st.begin(), st.end(), '\n'), st.end());
        if (!isNewlineString(st) && st != "") {
            nodeid = depthtable[depth - 1].back();
            nodes[nodeid - 1]->Content.append(st);
        }
        c = readchar(readFile, EOFread);
        if (c == '!' || c == '?') { // Comments or DOCTYPE or svg
            c = readchar(readFile, EOFread);
            if (c == '-') {
                while (1) {
                str = readtilldelim(readFile, '>', EOFread);
                if (str[str.size() - 1] == '-' && str[str.size() - 2] == '-')
                    break;
                }
            }
            else
            {
                str = readtilldelim(readFile, '>', EOFread);
            }
        }
        else if (c == '/') { // end tag!
            readtilldelim(readFile, '>', EOFread);  

            depth-- ;
            if (depth != 0) {
                nodeid = (depthtable[depth - 1]).back();
                nodes[nodeid - 1]->kids.insert(nodes[nodeid - 1]->kids.end(), depthtable[depth].begin(), depthtable[depth].end()   );  
                (depthtable[depth]).clear();
            }
        }
        else { // start or self-closing tag!
            temp = new node;
            word = readTag(readFile, tagattr, EOFread);
            word.insert (0, 1, c);
            temp->id = ct;
            temp->name = word;

            selfclosing = false;
            if (tagattr ) {
                val = readtilldelim(readFile, '>', EOFread);
                if (isWSstring(val) ){
                    val = "";
                }
                else if (val[val.length() - 1] == '/') {
                    val[val.length() - 1] = ' ';
                    selfclosing = true;
                }
            }
            else {
                val = "";
            }

            temp->value = val;
            depthtable[depth].push_back(temp->id);
            depth++;
            nodes.push_back(temp);
            ct++; 
                
            if (selfclosing) {    
                depth-- ;
                if (depth != 0) {
                    nodeid = (depthtable[depth - 1]).back();
                    nodes[nodeid - 1]->kids.insert(nodes[nodeid - 1]->kids.end(), depthtable[depth].begin(), depthtable[depth].end()   );  
                    (depthtable[depth]).clear();
                }                  
            }

          //  cout << t << endl;
        }
        // cout << "TABLE:\n";
        // printArrayofVectors( depthtable, depth);
        
        // if (ct >= 26 )
        //     cout << ct << " " << (depthtable[1])[0] << endl;

    }

    printNodes ( nodes, singleSplit(argv[1], ".").first );


    // Free objects;
    for (vector<struct node*>::iterator it = nodes.begin() ; it != nodes.end(); ++it) {
        free(*it);
    }

    readFile.close();

    return 0;
}


string readtilldelim (fstream& file, const char delim, bool& EOFread) {
    char buff;
    string str = "";

    file.get(buff);

    while( buff != delim) {
        str.append(1, buff);
        if ( !file.get(buff) ) { 
            EOFread = true;
            break;
        }
    }
    return str;   
}


char readchar (fstream& file, bool& EOFread) {
    char buff;
    
    buff = ' ';
    while( buff == ' ' || buff == '\n') {
        file.get( buff) ;
    }
         
    return buff;   
}

string readTag (fstream& file, bool& tagattr, bool& EOFread) {
    string str = "";
    char buff;
    
    file.get(buff);
  
    while (buff != ' ' && buff != '\n' && buff != '>' ) {
        str.append(1, buff);
        file.get(buff);
  
    }   

    tagattr = !(buff == '>') ;

    return str;
}

void printNodes (vector<struct node*> nodes, string filename) {
    long filetype = -1;
    vector<string> entries;
    pair<string,string> keyval;

    cout << "[" << endl;
    for (long i = 0; i < nodes.size(); i ++) {
        cout << "  {" << endl;
        cout << "\t\"Id\": " << nodes[i]->id << " ," << endl;
        string temp = nodes[i]->name;
        if (temp == "html")
            filetype = 0;
        else if (temp == "svg")
            filetype = 1;
        if (filetype == 1 && temp.find_first_of(':', 0) != string::npos) {
            long position = temp.find_first_of(':', 0); // Removing 'dc:' prefix from value of 'Name' tag for svg files.
            nodes[i]->name = temp.substr( position + 1 , temp.length() - position - 1);
        }
        cout << "\t\"Name\": \"" << nodes[i]->name <<  "\" ," << endl;
        cout << "\t\"Kids\": [" ;
        for (long j = 0; j < (nodes[i]->kids).size(); j++ ) {
            cout << " " << (nodes[i]->kids)[j];
            if (j != (nodes[i]->kids).size() - 1)
                cout << "," ;
        }
        cout << " ] ," << endl;
        // cout << "\t\"Value\": " ; 
        
        if (nodes[i]->value == "" || nodes[i]->value == " ")
            // cout << " { }," << endl;
            ;
        else
        {
            // cout << endl << "\t\t{ " << endl;
            entries = split( nodes[i]->value, " ");
            for (long j = 0; j < entries.size(); j++ ) {
                keyval = singleSplit(entries[j], "=");
                // .json files don't allow '\', '/' and additional '"' charactars. 
                
                if (isNewlineString(keyval.first) )
                    continue;
                changecharinString( keyval.first, '\"', '-');
                changecharinString( keyval.first, '/', '-');
                changecharinString( keyval.first, '\\', '-');   
                changecharinString( keyval.first, '\r', ' ');
                changecharinString( keyval.first, '\n', ' ');                
                    
                changecharinString( keyval.second, '\"', ' ');
                changecharinString( keyval.second, '/', ' ');
                changecharinString( keyval.second, '\\', ' '); 
                changecharinString( keyval.second, '\r', ' ');
                changecharinString( keyval.second, '\n', ' ');  
                if (isNewlineString(keyval.second))
                    keyval.second = "";

                if (keyval.first == "id" || keyval.first == "href")  // Trimming whitespaces for id and ref, else they might not match.
                    keyval.second = trimWS(keyval.second);
                cout << "\t\"" << keyval.first << "\": \"" << keyval.second << "\""; 
                // if (j != entries.size() - 1)
                    cout << ", " << endl;
                // else
                //     cout << endl;
            }
            // cout << "\t\t}," << endl;
        }
        // .json files don't allow '\', '/' and additional '"' charactars. 
        changecharinString(nodes[i]->Content, '\"', '-');
        changecharinString(nodes[i]->Content, '/', '-');
        changecharinString(nodes[i]->Content, '\\', '-');
        if (isNewlineString(nodes[i]->Content))
            nodes[i]->Content = "";

        cout << "\t\"Content\": \"" << nodes[i]->Content << "\"" << endl;  
        if (i != nodes.size() - 1)
            cout << "  }" << endl << "  ," << endl;
        else
            cout << "  }" << endl;
    }
    cout << "]" << endl;

}

bool isNewlineString (string s) {
    for (string::size_type i = 0; i < s.size(); i++) {
        if (s[i] != '\r' && s[i] != '\n' && s[i] != ' ')
            return false;
    }
    return true;
}

void changecharinString (string& s, char init, char final) {
    for (string::size_type i = 0; i < s.size(); i++) {
        if (s[i] == init)
            s[i] = final;
    }
    return; 
}

bool isWSstring (string s) {
    long i;
    for (i = 0; i < s.length(); i++) {
        if (s[i] != ' ' && s[i] != '\r' && s[i] != '\n' && s[i] != '\t')
            return false;
    }
    return true;
}

string trimWS (string s) {
    string rv;
    long i, j;
    for (i = 0; (isWS( s[i])) && (i < s.length()) ; i++);
    for (j = s.length() - 1; (s[j] == ' ' || s[j] == '\n') && (j >= 0 ) ; j--) ;
    if (i == s.length() || j == -1)
        return "";
    for (; i <= j ; i++) {
        rv.append(1, s[i]);
    }
    return rv;
}

bool isWS (char c) {
    return (c == ' ' || c == '\n' || c == '\t' || c == '\r') ;
}