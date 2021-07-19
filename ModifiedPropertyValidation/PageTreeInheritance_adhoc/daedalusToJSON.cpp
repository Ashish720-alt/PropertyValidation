#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "macros.hpp"

// If get a segfault with this file, then increase the value of macro LINESIZE in macros.hpp

#define OUTPUTFILE "JSONoutput.json"   // Will have to change this final name!

using namespace std;

bool removeUptodate ( char* s) ;
void removeDollar(char *s);
void insertKeyQuotes (char *s);
void append(char subject[], const char insert[], int pos);

int main( int argc, char *argv[] )  {

    if ( argc != 2) {
        fprintf(stderr, "Incorrect arguments: Give path to Daedalus output file!\n"); 
    }

    char *str = (char *) malloc(3 * LINESIZE * sizeof(char));
    FILE *infile = fopen( argv[1] , "r");
    FILE *outfile = fopen ( OUTPUTFILE , "w"); 
    while(1) {   
        fgets( str , LINESIZE , infile);
        if (feof(infile))
            break;
  
        if (removeUptodate (str))
            continue;
        insertKeyQuotes( str); 

        fprintf(outfile, "%s", str) ;    
    }    
    free(str); 
    fclose(infile);
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
    for (int i = 0; s[i] != '\n' ; i++) { 

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
    
        if (word == 0  && (isalpha(s[i]) || s[i] == '$') && valstring == 0) {
            append(s, "\"", i);
            i++;
            word = 1;
            continue;
        }
            
        if (word == 1 && !(isalpha(s[i]) || isdigit(s[i]) || s[i] == '\\' ) )  { 
            append(s, "\"", i); 
            i++;
            word = 0;
            continue;
        }
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