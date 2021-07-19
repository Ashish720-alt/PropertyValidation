
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
#include<stack>
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
void PrintStack(stack<string> s);

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

/* Open tags: Any tag after an open tag (tag without end tag) is considered a child of the open tag
until you reach the end tag of the parent tag of the open tag.*/


int main(int argc, char const *argv[])
{
    
    if ( argc != 2) {
        cerr << "Incorrect arguments: Give path to IR file!\n";
    }
    
    fstream readFile;
    
    readFile.open( argv[1], ios::in);
    char c;
    bool EOFread = false;
    string word;
    stack<string> tagsread;
    queue<string> opentags;
    string temp;
    bool tagattr = false;
    bool selfclosing;
    string val;
    string str;
    long i = 1;
// In comments, you could have '<, >' symbol.


    while (readFile.peek() != EOF ) {
        // PrintStack(tagsread) ;
        // cout << endl << "%%%%%%%%%%%%%%%%%%%%" << endl;
        i++;
        readtilldelim(readFile, '<', EOFread);
        if (EOFread)
            break;

        c = readchar(readFile, EOFread);
        if (c == '!') { // Comments or DOCTYPE
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
          word = readtilldelim(readFile, '>', EOFread);
        if (tagsread.empty())
            cerr << "Somethings not right\n" << i << endl << ct << endl;          
          while (word.compare( tagsread.top()) != 0 ) {
              temp = tagsread.top();
              tagsread.pop();
              if (tagsread.empty()) {
                 cerr << "Somethings not right\n" << i << endl <<  ct << endl;
              }
              opentags.push(temp);
          }
          tagsread.pop();  
        }
        else { // start tag!
            ct++;
            word = readTag(readFile, tagattr, EOFread);
            word.insert (0, 1, c);
            tagsread.push(word);  

            selfclosing = false;
            if (tagattr) {
                val = readtilldelim(readFile, '>', EOFread);
                if (val[val.length() - 1] == '/') {
                    selfclosing = true;
                }
            }
            
            if (selfclosing) {
                tagsread.pop();
            }
    
        }

    }



    while ( !opentags.empty()) {
        temp = opentags.front();
        opentags.pop();
        cout << temp << endl;
    }

    readFile.close();

    return 0;
}

void PrintStack(stack<string> s)
{
    // If stack is empty then return
    if (s.empty()) 
        return;
     
 
    string x = s.top();
 
    // Pop the top element of the stack
    s.pop();
 
    // Recursively call the function PrintStack
    PrintStack(s);
 
    // Print the stack element starting
    // from the bottom
    cout << x << " ";
 
    // Push the same element onto the stack
    // to preserve the order
    s.push(x);
}



string readtilldelim (fstream& file, const char delim, bool& EOFread) {
    char buff;
    string str = "";

    if (!file.get(buff)) {
        EOFread = true;
        return str;
    }

    while( buff != delim) {
        str.append(1, buff);
        if (!file.get( buff)) { 
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
    filename.append(".json"); // output file extension!
    ofstream writeFile ;
    writeFile.open(filename.c_str() );
    writeFile << "[" << endl;
    vector<string> entries;
    pair<string,string> keyval;

    for (long i = 0; i < nodes.size(); i ++) {
        writeFile << "  {" << endl;
        writeFile << "\t\"Id\": " << nodes[i]->id << " ," << endl;
        writeFile << "\t\"Name\": \"" << nodes[i]->name <<  "\" ," << endl;
        writeFile << "\t\"Kids\": [" ;
        for (long j = 0; j < (nodes[i]->kids).size(); j++ ) {
            writeFile << " " << (nodes[i]->kids)[j];
            if (j != (nodes[i]->kids).size() - 1)
                writeFile << "," ;
        }
        writeFile << " ] ," << endl;
        writeFile << "\t\"Value\": " ; 
        
        if (nodes[i]->value == "" || nodes[i]->value == " ")
            writeFile << " { }," << endl;
        else
        {
            writeFile << endl << "\t\t{ " << endl;
            entries = split( nodes[i]->value, " ");
            for (long j = 0; j < entries.size(); j++ ) {
                keyval = singleSplit(entries[j], "=");
                // .json files don't allow '\', '/' and additional '"' charactars. 
                
                if (isNewlineString(keyval.first) )
                    continue;
                changecharinString( keyval.first, '\"', '-');
                changecharinString( keyval.first, '/', '-');
                changecharinString( keyval.first, '\\', '-');                 
                    
                changecharinString( keyval.second, '\"', '-');
                changecharinString( keyval.second, '/', '-');
                changecharinString( keyval.second, '\\', '-'); 
                changecharinString( keyval.second, '\r', ' ');
                changecharinString( keyval.second, '\n', ' ');  
                if (isNewlineString(keyval.second))
                    keyval.second = "";

                writeFile << "\t\t  \"" << keyval.first << "\": \"" << keyval.second << "\""; 
                if (j != entries.size() - 1)
                    writeFile << ", " << endl;
                else
                    writeFile << endl;
            }
            writeFile << "\t\t}," << endl;
        }
        // .json files don't allow '\', '/' and additional '"' charactars. 
        changecharinString(nodes[i]->Content, '\"', '-');
        changecharinString(nodes[i]->Content, '/', '-');
        changecharinString(nodes[i]->Content, '\\', '-');
        if (isNewlineString(nodes[i]->Content))
            nodes[i]->Content = "";

        writeFile << "\t\"Content\": \"" << nodes[i]->Content << "\"" << endl;  
        if (i != nodes.size() - 1)
            writeFile << "  }" << endl << "  ," << endl;
        else
            writeFile << "  }" << endl;
    }
    writeFile << "]" << endl;
    writeFile.close();
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