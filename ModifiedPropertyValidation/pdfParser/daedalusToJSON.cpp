#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <iostream>
#include <stack>  
#include <sstream> 
#include <map> 
#include <set>
#include <iterator> 
#include <utility>
#include <queue>
#include <iostream>

// If get a segfault with this file, then increase the value of macro LINESIZE in macros.hpp

#define LINESIZE 300000

using namespace std;

bool removeUptodate ( char* s) ;
void removeDollar(char *s);
void insertKeyQuotes (char *s);
void append(char subject[], const char insert[], int pos);
void removedoublebrace (string& s);
void removeemptybrace (string& s) ;
void extractvaluedict ( string& s) ;
void extractstreamdict ( string& s) ;
string constructID (string& s );
char getignoreWS (string& s, long& pos);
string readstring (string& s, long& pos);
string readValueArray (string& s, long& pos);
string readValueDict (string& s, long& pos);
string readValueInt (string& s, long& pos);
char peekignoreWS (string& s, long& pos);
long stringToInt (string s);
long findPosInVectors ( vector <pair<long, long> > vp, pair<long, long> p );
string indent (string& s);
string changerefToID (string& s, vector <pair<long, long> > vp);
bool checkref(string s, long& pos, vector <pair<long, long> > vp, long& id ) ;
template <class T> inline string to_string (const T& t);
void fixbrokendictionary (string& s);

int main( int argc, char *argv[] )  {

    string doc, doc1; // can handle documents upto 4 GB in size!!!
    vector < pair<long, long> > idlist;
    
    if ( argc != 2) {
        fprintf(stderr, "Incorrect arguments: Give path to Daedalus output file!\n"); 
    }

    char *str = (char *) malloc(3 * LINESIZE * sizeof(char));
    FILE *infile = fopen( argv[1] , "r");
    
    while(1) {   
        fgets( str , LINESIZE , infile);
        if (feof(infile))
            break;
  
        // Remove the string "Up to date"
        if (removeUptodate (str))
            continue;
        
        // Add quotes to keys, change forward slash to backward slashes, and change '.' to ' ', remove types
        insertKeyQuotes( str); 
        doc.append( str);
           
    }    
    free(str); 
    fclose(infile);

    // Removes doublebraces created by removing types!
    removedoublebrace(doc);
    // Removes empty braces;
    removeemptybrace(doc);
    // Extract fields from "value" dictionary if field is a dictionary itself only!
    extractvaluedict(doc);
    // Extract fields from "stream" dictionary if field is a dictionary itself only!
    extractstreamdict(doc);
    
    // Add id tag, change refs to refer to ID tags, and indent!
    doc1 = constructID(doc);
    

    
    string s = argv[1];
    s = s.substr(0, s.find("."));
    s.append(".json");
    FILE *outfile = fopen ( s.c_str() , "w");     
    fprintf(outfile, "%s", doc1.c_str()) ; 
    fclose(outfile);
    
    return 0;
}

bool removeUptodate ( char* s) 
{
    if ( !strcmp(s, "Up to date\n"))
        return true;
    return false;
}

void insertKeyQuotes (char *s)
{
    bool word = 0;
    int valstring = 0;
    bool typestring = 0;
    long i;
    for ( i = 0; s[i] != '\n' ; i++) { 

        if (s[i] == '\\' ) {  // Change forward slashes in a word, else .json shows error. 
            s[i] = '/' ; 
        }
        else if (s[i] == '.' ) {  // Change . to ' ' as . occurs after number only without a decimal part eg: 538. , 3.
            s[i] = ' ' ; 
        }
        
        if (s[i] == '\"')
            if (valstring == 0)
                valstring ++;
            else
                valstring --;
    
        if (word == 0 && typestring == 0 && s[i] == '$' && valstring == 0) {
            typestring = 1;
            s[i] = ' ';
            continue;
        }

        if (word == 0 && typestring == 1 && (isalpha(s[i]) || isdigit(s[i]) || s[i] == ':') ) {
            s[i] = ' ';
            if (s[i] == ':')
                typestring = 0;
            continue;
        }

        if (word == 0 && typestring == 1 && !(isalpha(s[i]) || isdigit(s[i])  ) ) {
            typestring = 0;
            continue;
        }
        
        
        if (word == 0  &&  typestring == 0 && isalpha(s[i])  && valstring == 0) {
            append(s, "\"", i);
            i++;
            word = 1;
            continue;
        }
            
        if (word == 1 && typestring == 0 && !(isalpha(s[i]) || isdigit(s[i]) || s[i] == '_') || s[i] == '\\' )   { 
            append(s, "\"", i); 
            i++;
            word = 0;
            continue;
        }
    }
    if (word == 1 && typestring == 0 && valstring == 0) {
        append(s, "\"", i);   
        i++;
        word = 0;
    }


}

void append(char subject[], const char insert[], int pos) {
    char buf[3 * LINESIZE] = {};
    strncpy(buf, subject, pos); 
    int len = strlen(buf);
    strcpy(buf+len, insert); 
    len += strlen(insert);  
    strcpy(buf+len, subject+pos);
    strcpy(subject, buf);   
}

void removedoublebrace (string& s) {
    long brace = 0;
    bool doublebrace = 0;
    stack<long> bracepos;

    for (long i = 0; i < s.length(); i++) { 
        if (s[i] == '{' && doublebrace == 0)  {   
            doublebrace = 1;
            brace ++;
        }
        else if (s[i] == '{' && doublebrace == 1) {
            s[i] = ' ';
            doublebrace = 1;
            bracepos.push(brace);
            brace ++;
        }
        else if ( !( s[i] == '}' || s[i] == '{' || s[i] == ' ' || s[i] == '\n') && doublebrace == 1) {
            doublebrace = 0;
        }
        else if ( s[i] == '}' && doublebrace == 1) {
            doublebrace = 0;
            brace --;
            if (  !bracepos.empty()  && bracepos.top() == brace) {
                bracepos.pop();
                s[i] = ' ';
            }
        }
        else if ( s[i] == '}' && doublebrace == 0) {
            brace --;
            if (  !bracepos.empty()  && bracepos.top() == brace) {
                bracepos.pop();
                s[i] = ' ';
            }
        }
        
    } 

    return;
}

void removeemptybrace (string& s) {
    bool brace = 0;
    long pos = -1;
    for (long i = 0; i < s.length(); i++ ) {
        if ( s[i] == '{') {
            brace = 1;
            pos = i;
        }
        else if ( s[i] != '{' && brace == 1) {
            
            if ( s[i] == '}') {
                s[i] = ' ';
                s[pos] = ' ';
                pos = -1;
                brace = 0;
            }
            else if ( s[i] == ' ' || s[i] == '\n') { }   
            else {
                brace = 0;
                pos = -1;
            }         
        }
        else {  }

    }
    return;
}

void extractvaluedict ( string& s) {
    bool foundvalue = 0;
    long pos1 = -1;
    long pos2 = -1;
    long pos3 = -1;
    long pos4 = -1;
    long cb, sb;

    for ( long i = 0; i < s.length(); i++ ) {
        if (s[i] == '\"') {
            pos1 = i;
            i++;
            if (s[i] != 'v') {
                for ( ; s[i] != '\"'; i++ ) {
                }
                    continue;
                
            }
            i++;
            if (s[i] != 'a') {
                for ( ; s[i] != '\"'; i++ );
                continue;
            }
            i++;
            if (s[i] != 'l') {
                for ( ; s[i] != '\"'; i++ );
                continue;
            }
            i++;
            if (s[i] != 'u') {
                for ( ; s[i] != '\"'; i++ );
                continue;
            }
            i++;
            if (s[i] != 'e') {
                for ( ; s[i] != '\"'; i++ );
                continue;
            }
            i++;
            if (s[i] != '\"') {
                for ( ; s[i] != '\"'; i++ );
                continue;
            }
            i++;
            
            while (s[i] == ' ' || s[i] == '\n') {
                i++;
            }
            if (s[i] != ':') {
                continue;
            }
            pos4 = i;
            i++;
            while (s[i] == ' ' || s[i] == '\n') {
                i++;
            }
            if (s[i] != '{') {
                if (s[i] == '\"') // Happens when value is a string, don't extract!
                    i--;
                continue;
            }                        
            pos2 = i;   
            cb = 1; 
            sb = 0;
            bool brokendictcolon = true;
            bool brokendictcomma = true;
            while ( !(cb == 0 && sb == 0) ) {
                i++;
                if (s[i] == '{')
                    cb++;
                if (s[i] == '}')
                    cb--;   
                if (s[i] == '[')
                    sb++;
                if (s[i] == ']')
                    sb--;                 
                if (s[i] == ':' )
                    brokendictcolon = false;
                if (s[i] == ',')
                    brokendictcomma = false;
            }
            if (brokendictcolon && brokendictcomma)
                continue;
            pos3 = i;
            s[pos2] = ' ';
            s[pos3] = ' ';
            for (long j = 0; j <= 6; j++)
                s[pos1 + j] = ' ';
            s[pos4] = ' ';
        }
    }
    
    return;
}

void extractstreamdict ( string& s) {
    bool foundvalue = 0;
    long pos1 = -1;
    long pos2 = -1;
    long pos3 = -1;
    long pos4 = -1;
    long cb, sb;

    for ( long i = 0; i < s.length(); i++ ) {
        if (s[i] == '\"') {
            pos1 = i;
            i++;
            if (s[i] != 's') {
                for ( ; s[i] != '\"'; i++ );
                continue;
            }
            i++;
            if (s[i] != 't') {
                for ( ; s[i] != '\"'; i++ );
                continue;
            }
            i++;
            if (s[i] != 'r') {
                for ( ; s[i] != '\"'; i++ );
                continue;
            }
            i++;
            if (s[i] != 'e') {
                for ( ; s[i] != '\"'; i++ );
                continue;
            }
            i++;
            if (s[i] != 'a') {
                for ( ; s[i] != '\"'; i++ );
                continue;
            }
            i++;
            if (s[i] != 'm') {
                for ( ; s[i] != '\"'; i++ );
                continue;
            }            
            i++;
            if (s[i] != '\"') {
                for ( ; s[i] != '\"'; i++ );
                continue;
            }
            i++;
            
            while (s[i] == ' ' || s[i] == '\n') {
                i++;
            }
            if (s[i] != ':') {
                continue;
            }
            pos4 = i;
            i++;
            while (s[i] == ' ' || s[i] == '\n') {
                i++;
            }
            if (s[i] != '{') {
                continue;
            }                        
            pos2 = i;   
            cb = 1; 
            sb = 0;
            while ( !(cb == 0 && sb == 0) ) {
                i++;
                if (s[i] == '{')
                    cb++;
                if (s[i] == '}')
                    cb--;   
                if (s[i] == '[')
                    sb++;
                if (s[i] == ']')
                    sb--;                 
            }
            pos3 = i;
            s[pos2] = ' ';
            s[pos3] = ' ';
            for (long j = 0; j <= 7; j++)
                s[pos1 + j] = ' ';
            s[pos4] = ' ';
        }
    }
    
    return;
}

string constructID (string& s ) {

     // replace references by ID;
    string key;
    string valtemp;
    string value;
    char id, id1, id2;
    long pos = -1;
    bool writeid = 1;
    string doctemp, doctemp2;
    bool first = 0;

    id2 = getignoreWS( s, pos ); // id2 = '['
    doctemp.append("[\n");
    while (id2 != ']') {
        if (first) {
            doctemp.append(",\n");
        }
        else {
            first = 1;
        }
        id1 = getignoreWS( s, pos); // id1 = '{'
        while (id1 != '}') {  
            key = readstring(s, pos);
            getignoreWS(s, pos ); // expect s[pos] =':'
            id = peekignoreWS(s, pos);
            if (id == '\"') {
                valtemp = readstring(s, pos);
            }
            else if (id == '[') {
                valtemp = readValueArray(s, pos);
                fixbrokendictionary(valtemp);
            }
            else if (id == '{') {
                valtemp = readValueDict(s, pos);
                fixbrokendictionary(valtemp);
            }
            else if ( isdigit(id) || id == '-') {
                valtemp = readValueInt(s, pos);
            }
            else if (id == '}' || id == ',') { // Value is an empty string!
                valtemp = "\"\"";
            }
            id1 = getignoreWS(s, pos);  // expect ',' or '}'  
            if (writeid) {
                doctemp.append("  {\n");
                writeid = 0;
            }
            value = indent (valtemp);
            if (key != "\"obj\"") {
                doctemp.append("  , ");
            }
            else
            {
                doctemp.append("    ");
            }
             // First field!
            doctemp.append(key);
            doctemp.append(": ");
            doctemp.append(value);
            doctemp.append(1, '\n');
        }
        doctemp.append("  }\n");
        writeid = 1;
        id2 = getignoreWS(s, pos);
    }
    doctemp.append("]\n");
    return doctemp; 

}

char peekignoreWS (string& s, long& pos) {
    pos++;
    while ( s[pos] == ' ' || s[pos] == '\n') {
        pos++; 
    }
    pos--;
    return s[pos + 1];
}

char getignoreWS (string& s, long& pos) {
    pos++;
    while ( s[pos] == ' ' || s[pos] == '\n') {
        pos++; 
    }
    return s[pos];
}

string readstring (string& s, long& pos) {
    char c = getignoreWS( s, pos);
    string text(1, c);
    c = 'a';
    while ( c != '\"') {
        pos++;
        c = s[pos];
        text.append(1 ,c);
    }
    return text;
}

string readValueArray (string& s, long& pos) {
    char c = getignoreWS( s, pos);
    long cb = 0;
    long sb = 1;
    string value(1, c);
    c = 'a';
    while ( c != ']' || cb != 0 || sb != 0) {
        pos++;
        c = s[pos];
        if (c == '[')
            sb++;
        else if (c == ']')
            sb--;
        else if (c == '{')
            cb++;
        else if (c == '}')
            cb--;
        value.append(1 ,c);
    }
    return value;
}

string readValueDict (string& s, long& pos) {
    char c = getignoreWS( s, pos);
    long cb = 1;
    long sb = 0;
    string value(1, c);
    c = 'a';
    while ( c != '}' || cb != 0 || sb != 0) {
        pos++;
        c = s[pos];
        if (c == '[')
            sb++;
        else if (c == ']')
            sb--;
        else if (c == '{')
            cb++;
        else if (c == '}')
            cb--;
        value.append(1 ,c);
    }
    return value;
}

string readValueInt (string& s, long& pos) {
    bool neg = false;
    char c = getignoreWS( s, pos);
    string value;
    if (c == '-') {
        neg = true;
        value.append(1, c);
        c = '0';
    }
    while ( isdigit(c) ) {
        if (!neg) {
            value.append(1 ,c);
        }
        else {
            neg = false;
        }

        pos++;
        c = s[pos];
    }
    pos--;
    return value;
}

long stringToInt (string s) {
    stringstream temp;
    long num;
    temp.str(s);
    temp >> num;
    return num;
}

long findPosInVectors ( vector <pair<long, long> > vp, pair<long, long> p ) {
    for (long pos = 0 ; pos < vp.size() ; pos++) {
        if ( (vp[pos]).first == p.first && (vp[pos]).second == p.second ) {
            return pos;
        }
    }
    return -1;
}

string indent (string& s) {
    string s2, s3;
    long pos;

    for (long i = 0; i < s.length(); i++) {
        if (s[i] == '\n') 
            s[i] = ' ';
    }

    for  (long i = 0; i < s.length(); i++) {
        if (s[i] != ' ') {
            s2.append(1, s[i]);
            continue;
        }
        else {
            for ( ; s[i] == ' '; i++) ;
            i--;
            s2.append(1, ' ');
            continue;
        }
    }

    for (long i = 0; i < s2.length() ; i++) {
        if (s2[i] == ':') {
            s3.append(1, s2[i]);
            pos = i;
            i++;
            for ( ; s2[i] == ' ' ; i++);
            if (s2[i] == ',') {
                s3.append(" \"\" ,");
                continue;
            }
            i = pos;
            continue;
        }
        s3.append(1, s2[i]);
    }

    return s3;
}

bool checkref(string s, long& pos, vector <pair<long, long> > vp, long& id ) {
    long num1, num2;
    id = 0;
    if (s[pos] != '{')
        return false;
    if (readstring(s, pos) != "\"obj\"" )
        return false;    
    if (getignoreWS(s, pos) != ':')
        return false;
    num1 = stringToInt (readValueInt( s, pos));
    if (getignoreWS(s, pos) != ',')
        return false;
    if (readstring(s, pos) != "\"gen\"" )
        return false;    
    if (getignoreWS(s, pos) != ':')
        return false;
    num2 = stringToInt (readValueInt( s, pos));
    if (getignoreWS(s, pos) != '}')
        return false;
    id = findPosInVectors(  vp,  make_pair(num1, num2 ) );
    return true;
}

template <class T> inline string to_string (const T& t)
{
    stringstream ss;
    ss << t;
    return ss.str();
}

void fixbrokendictionary (string& s) {
    long cb;
    long sb;
    long j;
    bool colon;
    bool comma;
    bool onlyWS;
    for (long i = 0; i < s.length() ; i++ ) {
        if (s[i] == '{') {
            cb = 1;
            sb = 0;
            j = i + 1;
            colon = false;
            comma = false;
            onlyWS = true;
            for (; !(s[j] == '}' && cb == 1 && sb == 0) ; j++ ) {
                if (s[j] != ' ' && s[j] != '\n' && s[j] != '}' )
                    onlyWS = false;
                if (s[j] == '{')
                    cb++;
                else if (s[j] == '}')
                    cb--;
                else if (s[j] == '[')
                    sb++;
                else if (s[j] == ']')
                    sb--;
                if (cb == 1 && sb == 0 && s[j] == ':')
                    colon = true;
                if (cb == 1 && sb == 0 && s[j] == ',')
                    comma = true;
                
            }
            if ( !colon && !comma) {
                if (!onlyWS) {
                    s[i] = ' ';
                    s[j] = ' ';
                }
                else {
                    s[i] = '\"';
                    s[j] = '\"';                    
                }
            }
            continue;
        }
    }
    return;        
}
