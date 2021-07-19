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
#include <time.h>
#include <ctype.h>
#include "macros.hpp"
#include "utility.hpp"
#include "debugging.hpp"

using namespace std;


// Function Prototypes
string readtext (fstream& file) ;
bool stringcomparerv (string s, const string s1);
void stringcompareabort (string s, const string s1);

int main(int argc, char const *argv[])
{
    if ( argc != 3) {
        cerr << "Incorrect arguments: Give path to IR file and Spec file!\n";
    }
    
    fstream IRfile;
    long sb = 0;
    long cb = 0;
    string text;
    string d;

    IRfile.open( argv[1], ios::in);
    text = readtext(IRfile);
    stringcompareabort(text, "[");
    sb++;
    d.clear();
    while (sb != 0 || cb != 0) {
        text = readtext(IRfile);

        if (text.compare("[") == 0)
            sb++;
        else if (text.compare("]") == 0)
            sb--;
        else if (text.compare("{") == 0)
            cb++;
        else if (text.compare("}") == 0) 
            cb--;                

        if (sb != 1 || cb != 0 || text.compare("}") == 0){
            d.append(text);
            if ((text.compare("}") == 0) && !(sb != 1 || cb != 0) ) {
                d.append("\n");
            }
        }


        if (sb == 1 && cb == 0) {
            cout << d;
            d.clear();
        }
    
    
    }


    IRfile.close();
    
    return 0;
}


string readtext (fstream& file) {
    string str = "";
    char buff;
    
    file.get( buff) ;
    if( buff == ',' || buff == ':' || buff == '{' || buff == '}' 
        || buff == '[' || buff == ']' ){
        str.append(1, buff);
        return str;
    }
    else if (buff == '\"') {
        str.append(1, buff);
        file.get(buff);
        while (buff != '\"') {
            str.append(1, buff);
            file.get(buff);
        }
        str.append(1, buff);
    }
    else if (isdigit)

    if (buff != '\"') {    
        while (buff != ' ' && buff != '\n' && buff != ',' && buff != ':' && buff != '{' && buff != '}'
            && buff != '[' && buff != ']' && buff != '\r') {
            str.append(1, buff);
            file.get(buff);
        }    
    }
    else {
        str.append(1, buff);
        file.get(buff);
        while (buff != '\"') {
            str.append(1, buff);
            file.get(buff);
        }
        str.append(1, buff);
    }
    
    return str;
}

bool stringcomparerv (string s, const string s1) {
    if (s.compare(s1) != 0) {
        cerr << "Unexpected file format, expected "<< s1 <<" here!\n";
        return false;
    }   
    return true;
}

void stringcompareabort (string s, const string s1) {
    if (s.compare(s1) != 0) {
        cerr << "Unexpected file format, expected "<< s1 <<" here!\n";
        abort();
    }   
    return ;
}