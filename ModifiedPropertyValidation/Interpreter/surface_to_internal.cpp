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
#include <stack> 
#include <time.h>
#include <ctype.h>
#include "macros.hpp"
#include "utility.hpp"
#include "printFunctions.hpp"
#include "surface_to_internal.hpp"

#define END_DELIMITER "-"




std::string surface_to_internal_spec (std::string surface_spec_filename) {
    
    fstream surface_spec;
    fstream internal_spec;
    long path_pos = surface_spec_filename.find_first_of('/', 0) + 1;
    long extension_pos = surface_spec_filename.find_first_of('.', path_pos ) ;
    std::string internal_spec_filename = surface_spec_filename.substr( path_pos, extension_pos - path_pos ) 
        + "__internal" + surface_spec_filename.substr(extension_pos, surface_spec_filename.size() - extension_pos);

    surface_spec.open( surface_spec_filename.c_str(), ios::in);
    internal_spec.open( internal_spec_filename.c_str(), fstream::out);  

    
    surface_spec.seekg(0,  std::ios::end);
    size_t size = surface_spec.tellg();
    std::string contents(size, ' ');
    surface_spec.seekg(0);
    surface_spec.read(&contents[0], size);

    std::string modified_string = spec_file_changes( contents );

    internal_spec << modified_string;

    surface_spec.close();
    internal_spec.close();

    return internal_spec_filename;

}

std::string spec_file_changes( std::string s) {
    s = trivial_modifications(s);
    
    

    s = parse_Spec(s);

    return s;
}





std::string parse_Spec (std::string s) {
    long i = 0;
    void initialize_operators( );
    while (i < s.length() ) {
        i = next_non_WS_char(s, i);
        if ( i >= s.length())
            break;
        long j = i;
        while ( i < s.length() && !isWS( s[i]) )
            i++;
        string section_type = s.substr(j, i - j - 1); // Don't want the terminal colon!
        i = next_non_WS_char(s,i);    
        bool spec_flag = false;
        while ( s[i] != '-') {
            if (s[i] == '%') {
                spec_flag = true;                
                i++;
                i = next_non_WS_char(s, i);
                continue;
            }

            std::string property_Spec =  trimWS(s.substr( i , s.find("..", i) - i ));   
            std::string new_property_Spec;
            if (section_type == "T" || section_type == "F" || section_type == "G" || section_type == "OT") { // Check for forests, graphs and ordered trees
                if (property_Spec.find("satisfying") != std::string::npos) {
                    vector<string> props = trimWS_vector( split(property_Spec, "where" ) );
                    string temp_property;
                    temp_property = trimWS((singleSplit(props[0], "satisfying" )).second);
                    props[0] = replace_substring(props[0] , temp_property, infix_to_prefix(temp_property)  );
                    for (long i = 1; i < props.size() ; i++) {
                        temp_property = trimWS((singleSplit(props[i], "satisfies" )).second);
                        props[i] = replace_substring(props[i] , temp_property, infix_to_prefix(temp_property)  );
                    }
                    new_property_Spec = props[0];
                    for (long i = 1; i < props.size() ; i++) {
                        new_property_Spec = new_property_Spec + " where " + props[i];
                    }
                }
                else {
                    if (section_type != "OT") {
                        std::string props = trimWS( singleSplit(property_Spec, "where" ).second );
                        new_property_Spec = replace_substring(property_Spec, props, infix_to_prefix( props ) );
                    }
                    else {
                        std::vector<std::string> props = trimWS_vector( split (property_Spec, "where" ) );
                        if (props.size() > 2) {
                            std::pair<string,string> props2 = trimWS_pair( singleSplit(props[2], "is" ) );
                            props2.second = infix_to_prefix(props2.second) ;
                            props[2] = props2.first + " is " + props2.second;
                        }
                        props[1] = infix_to_prefix(props[1]);
                        new_property_Spec = props[0];
                        for (long k = 1; k < props.size(); k++) {
                            new_property_Spec = new_property_Spec + " where " + props[k];
                        }                      
                    }
                } // Recursive Tree Property
            }
            else if (!spec_flag) {
                long not_required;
                if (read_next_word(property_Spec, 0 ,  not_required) == "forall") {
                    string props = trimWS( singleSplit(property_Spec, "=" ).second );
                    new_property_Spec = replace_substring(property_Spec, props, remove_unnecessary_brackets(" " +  infix_to_prefix( props )) );
                    new_property_Spec = add_id_fields_in_quantifiers(new_property_Spec);
                }
                else {
                    string props = trimWS(  (split (singleSplit(property_Spec, "satisfying" ).second, "where"))[0] );
                    string props2 = infix_to_prefix (props);
                    string props3 = remove_unnecessary_brackets(" " + props2);
                    new_property_Spec = replace_substring(property_Spec, props, remove_unnecessary_brackets(" " +  infix_to_prefix( props )) );
                }
            }
            else {
                vector<string> props_vector =  split(property_Spec, ":" ) ;
                string props = props_vector[ props_vector.size() - 1 ];
                new_property_Spec = replace_substring(property_Spec, props, remove_unnecessary_brackets(" " +  infix_to_prefix( props ) ) );
                new_property_Spec = add_id_fields_in_quantifiers(new_property_Spec);                
            }
            // std::cout << property_Spec << endl << new_property_Spec << endl;
            // if (!new_property_Spec.empty())
                s = replace_substring(s, property_Spec, new_property_Spec);
            i = s.find("..", i) + 2;
            i = next_non_WS_char(s, i);
        }
        i++;
    }
    return s;
}


std::string remove_unnecessary_brackets(std::string s) {
    std::string rv = s;
    std::string word;
    bool open_brace = false;
    long previous_bracket_position;
    long open_bracket_count = 0;
    std::stack<long> open_bracket_ct_when_deleting;
    long open_comma = false;

    // Change `( ( P ) )' -> `( P )' where P is some proposition and `( )' -> ' ' instance.
    for (long i = 0; i < rv.size() ; ) {
        for (  ; isWS( rv[i])  ; i++ ) 
            if (i == rv.size() )
                break;
        
        word.clear();
        if (rv[i] == '(' || rv[i] == ')'|| rv[i] == ',') {
            word.append(1, rv[i]);
            i++;
        }
        else {
            for (  ; !isWS(rv[i]) && !(rv[i] == '(' || rv[i] == ')' || rv[i] == ',') ; i++  ) {
                if (i == rv.size())
                    break;
                word.append(1 , rv[i]);
            }
            word.append(1, ' ');
        }

        //cout << word << endl;

        if (word == "(")
            open_bracket_count++;
        else if (word == ")")
            open_bracket_count--;
        

        
        if ((open_brace || open_comma) && (word == "(")) {
            rv[i-1] = ' '; 
            open_bracket_ct_when_deleting.push(open_bracket_count - 1);
        }    
        if (open_brace && (word == ")")) {
            rv[i-1] = rv[previous_bracket_position] = ' ';
        }
        if (!open_bracket_ct_when_deleting.empty() && open_bracket_count == open_bracket_ct_when_deleting.top() && word == ")") {
            rv[i-1] = ' ';
            open_bracket_ct_when_deleting.pop();
        }
        open_brace = (word == "("); 
        open_comma = (word == ",");
        if (word == "(")
            previous_bracket_position = i - 1;

    }



    return rv;
}



std::string add_id_fields_in_quantifiers ( std::string P) {
    std::vector<string> props = trimWS_vector ( splitavoidarrdictfun( P, ":" ) );
    std::string retval;
    std::string temp;

    for (long i = 0; i < props.size() - 1; i++) {
        temp = props[i];
        long pos = 0;
        if ( temp.find( "Child", 0) != std::string::npos ) {
            long newpos;
            pos = temp.find( "Child", 0);
            string arg = trimWS(readstringbetween( read_next_word(temp, pos + 5, newpos ) , '(', ')' ));
            temp = temp.substr(0, pos) + "Child( " + arg + ".<\"Id\"> )";
        }
        else if ( temp.find( "PATH", 0) != std::string::npos ) {
            long newpos;
            pos = temp.find( "PATH", 0);
            std::string arg = trimWS(readstringbetween( read_next_word(temp, pos + 4, newpos ) , '(', ')' ));
            std::vector<std::string> args = trimWS_vector( splitavoidarrdictfun( arg, "," ) );
            arg = "( " + args[0] + ".<\"Id\">" + " , ";
            for (long j = 1; j < args.size() ; j++ ) {
                arg = arg + args[j];
            }
            arg += " )";
            temp = temp.substr(0, pos) + "PATH " + arg;
        }
        retval = retval + temp + ":\n";
    }
    retval = "\n" + retval + props[ props.size() - 1];

    return retval;
}




std::string trivial_modifications (std::string s) {
    size_t end_delimiter_size = sizeof(END_DELIMITER) - 1;

    s = replace_all_substr(s, "Semantic Definitions", "SD" );
    // s = replace_all_substr(s, "Attributes", "SD" );
    // s = replace_all_substr(s, "Graph Property", "SD" ); // Remove this for merging
    s = replace_all_substr(s, "Semantic Conditions:", "%" );

    s = replace_all_substr(s, "Ordered Tree", "OT");
    s = replace_all_substr(s, "Tree", "T");
    s = replace_all_substr(s, "Forest", "F");
    s = replace_all_substr(s, "Graph", "G");

    s = add_end_delimiter(s , "SD");
    // s = add_end_delimiter(s , "GP"); // Remove this for merging
    // s = add_end_delimiter(s , "OTP");
    long end_position = s.size();
    s.resize( s.size() + end_delimiter_size, ' ');
    s.insert(end_position, END_DELIMITER );

    s = add_second_period(s);
    s = modify_forall_delimiter(s);
    return s;

}


std::string modify_forall_delimiter (std::string s) {
    long cb = 0;
    long sb = 0;
    long ob = 0;
    bool instring = false;
    for (long i = 0; i < s.length(); i++) {
        if (s[i] == '\"')
            instring = !instring;
        else if (s[i] == '[' && !instring)
            sb++;
        else if (s[i] == ']' && !instring )
            sb--;
        else if (s[i] == '{' && !instring )
            cb++;
        else if (s[i] == '}' && !instring )
            cb--;    
        else if (s[i] == '(' && !instring )
            ob++;
        else if (s[i] == ')' && !instring )
            ob--;   
        else if (s[i] == ',' && ob == 0 && cb == 0 && sb == 0 && !instring)
            s[i] = ':';     
    }
    return s;
}


std::string add_second_period (std::string s) {
    long search_start = 0;
    long start_index;
    bool has_newline;
    while ( s.find( "." , search_start) != std::string::npos ) {
        start_index = s.find( "." , search_start);
        long check_pos = start_index + 1;
        has_newline = false;
        while ( isWS( s[check_pos]) ) {
            if ( s[check_pos] == '\n')
                has_newline = true;
            check_pos++;
        }
        if (!has_newline) {
            search_start = start_index + 1;
            continue;
        }
        s.resize( s.size() + 1, ' ');        
        s.insert(start_index+ 1, "." );
        search_start = start_index + 2;    
    }
    return s;    
}

std::string add_end_delimiter (std::string s, std::string new_section_string) {
    long len = new_section_string.size();
    long search_start = 0;
    long start_index, end_index;
    size_t end_delimiter_size = sizeof(END_DELIMITER) - 1;
    while ( s.find( new_section_string , search_start) != std::string::npos ) {
        start_index = s.find( new_section_string , search_start);
        end_index = start_index + len; // points to position after end of initial_substr occurrence
        if (  isalpha( s[end_index] ) || s[end_index] == '_'  ) {
            search_start = end_index;
            continue;
        }    
        
        s.resize( s.size() + end_delimiter_size, ' ');
        s.insert(start_index - 1, END_DELIMITER );
        search_start = end_index;   
    }
    return s;    
}



std::string replace_all_substr ( std::string s, std::string initial_substr, std::string final_substr) {
    long initial_len = initial_substr.size();
    long final_len = final_substr.size();
    long search_start = 0;
    long start_index, end_index;
    while ( s.find( initial_substr , search_start) != std::string::npos ) {
        start_index = s.find( initial_substr , search_start);
        end_index = start_index + initial_len; // points to position after end of initial_substr occurrence
        if (  isalpha( s[end_index] ) || s[end_index] == '_'  ) {
            search_start = end_index;
            continue;
        }    
        s.replace(start_index, initial_len, final_substr);
        search_start = start_index + final_len;    
    }
    return s;
}



void initialize_operators( std::map<string, long>& infix_operator_precedence, std::set<string>& prefix_operators) {
    infix_operator_precedence.insert( pair<string, long>("DEFAULT", -1) );    // implementation requires default operator value.    
    infix_operator_precedence.insert( pair<string, long>("or", 1) );
    infix_operator_precedence.insert( pair<string, long>("and", 1) );
    infix_operator_precedence.insert( pair<string, long>("implies", 1) );
    infix_operator_precedence.insert( pair<string, long>("in", 2) );    
    infix_operator_precedence.insert( pair<string, long>("union", 2) );  
    infix_operator_precedence.insert( pair<string, long>("=", 2) );
    infix_operator_precedence.insert( pair<string, long>("!=", 2) );
    infix_operator_precedence.insert( pair<string, long>(">=", 2) );
    infix_operator_precedence.insert( pair<string, long>(">", 2) );
    infix_operator_precedence.insert( pair<string, long>("<=", 2) );
    infix_operator_precedence.insert( pair<string, long>("<", 2) );

    prefix_operators.insert("inn");
    prefix_operators.insert("ins");
    prefix_operators.insert("eqn");
    prefix_operators.insert("eqs");
    prefix_operators.insert("iskeydefined");
    prefix_operators.insert("not");
    prefix_operators.insert("REFSTRING");
    prefix_operators.insert("PATH");
    prefix_operators.insert("make_singleton_array"); 
    prefix_operators.insert("append_all_children_attributes"); 
    prefix_operators.insert("isset"); 
    prefix_operators.insert("isempty"); 
    prefix_operators.insert("ochild_field"); 
    prefix_operators.insert("parent_field");
    prefix_operators.insert("grandparent_field");  
    prefix_operators.insert("indexn");
    prefix_operators.insert("ancestor");  
    return;
}


string infix_to_prefix (string s) {
    std::map<string, long> infix_operator_precedence;
    std::set<string> prefix_operators;
    initialize_operators( infix_operator_precedence, prefix_operators) ;

    std::stack<string> operator_stack;
    std::stack<string> operand_stack;

    long i = 0;
    while (i < s.length()) {
        std::string word = read_next_word(s, i, i);
        if (word.empty() && i == s.length())
            break;
        if ( prefix_operators.find( word) != prefix_operators.end()) { // Treat a prefix operator as a unary operator + operand
            if ( word == "iskeydefined") {
                std::string next_word = trimWS(read_next_word(s,i,i) );
                std::pair<std::string, std::string> args = singleSplit( readstringbetween( next_word , '(', ')' ) , "," );
                next_word = "( " + trimWS(infix_to_prefix ( args.first )) + " , <" + trimWS(infix_to_prefix( args.second) ) + "> )" ;
                
                operand_stack.push( word + next_word );
            }
            else {
                std::string next_word = trimWS(read_next_word(s,i,i) );
                std::vector<string> args = splitavoidarrdictfun( readstringbetween( next_word , '(', ')' ) , "," );
                std::string edited_next_word = "( " + infix_to_prefix ( args[0] );
                for (long i = 1; i < args.size(); i++) 
                    edited_next_word += " , " + infix_to_prefix ( args[i] );
                edited_next_word += " )";
                
                next_word = edited_next_word;
                operand_stack.push( word + next_word );
            }
        }
        else if (infix_operator_precedence.find( word) != infix_operator_precedence.end()  ) {

            if (word == "!=") {
                string op1 = operand_stack.top();
                operand_stack.pop();
                string op2 = read_next_word(s,i,i);
                string typed_operator = type_inference("=" , op1, op2);
                operand_stack.push("not( " + typed_operator + "( " + op1 + " , " + op2 + ") )"); 
            }
            else 
                operator_stack.push( word);
        }
        else if ( is_enclosed_by_delimiters(word, '(', ')') ) { // Just for braces, not function  arguments to prefix operators, that is handled in the prefix case!
            string temp_word =  readstringbetween( word, '(', ')' ) ;
            temp_word = infix_to_prefix(temp_word);
            temp_word = "( " + temp_word + " )";
            operand_stack.push (temp_word);
        } 
        else {

            word = trimWS( word );
            if ( !is_enclosed_by_delimiters(word, '[', ']' ) && !is_enclosed_by_delimiters(word, '<', '>' )  && !is_enclosed_by_delimiters(word, '\"', '\"' ) && !isStringNum(word)
                    && !(word == "True") && !(word == "False") ) {   // True and False are constants
                if (word.find(".", 0) == std::string::npos && !( word == "T" || word == "F" || word == "OT" || word == "G" ) ) // Replace reference to d by d.<"Id">
                    word = word + ".<\"Id\">";
            }
            operand_stack.push (word);
        }
    
    }

    while ( !operator_stack.empty() ) {
        std::stack<string> operator_stack_auxiliary;
        std::stack<string> operand_stack_auxiliary;

        operand_stack_auxiliary.push( operand_stack.top() );
        operand_stack.pop();


        string current_operator = operator_stack.top();
        operator_stack.pop();
        operator_stack_auxiliary.push( current_operator );

        string next_operator = (operator_stack.empty()) ? "DEFAULT" : operator_stack.top();

        operand_stack_auxiliary.push( operand_stack.top() );
        operand_stack.pop();



        // This assumes both operators are binary and infix
        while (  infix_operator_precedence[next_operator] > infix_operator_precedence[current_operator] ) {
            operator_stack_auxiliary.push( next_operator);
            operator_stack.pop();
            current_operator = next_operator;
    
            operand_stack_auxiliary.push( operand_stack.top() );
            operand_stack.pop();

            next_operator = (operator_stack.empty()) ? "DEFAULT" : operator_stack.top();
        }


        string temp_string =  operand_stack_auxiliary.top();
        operand_stack_auxiliary.pop();
        while (!operator_stack_auxiliary.empty()) {
            string operator_temp = operator_stack_auxiliary.top();
            operator_stack_auxiliary.pop();
            string operand_temp = operand_stack_auxiliary.top();
            operand_stack_auxiliary.pop();  
            if (operator_temp == "<")
                operator_temp = "l";
            else if (operator_temp == "<=")
                operator_temp = "le";
            else if (operator_temp == ">")
                operator_temp = "g";
            else if (operator_temp == ">=")
                operator_temp = "ge";
            if (operator_temp == "=" || operator_temp == "in")
                operator_temp = type_inference(operator_temp, temp_string, operand_temp);     
            temp_string = operator_temp + "( " + temp_string + " , " + operand_temp + " )";
        }
        operand_stack.push( temp_string );
    }

    return operand_stack.top();

}


string type_inference (string op, string arg1, string arg2) {
    string type_inferred_op = op;
    if (type_inferred_op == "=")
        type_inferred_op = "eq";
    
    arg1 = trimWS(arg1);
    arg2 = trimWS(arg2);
    long ind1 = 0;
    long ind2 = 0;
    string word1 = read_next_word( arg1, ind1, ind1 );
    string word2 = read_next_word( arg2, ind2, ind2 );

    if ( is_enclosed_by_delimiters( arg1, '\"', '\"' ) || arg1 == "True" || arg1 == "False" ||
         is_enclosed_by_delimiters( arg2, '\"', '\"' ) || arg2 == "True" || arg2 == "False" ) 
             return type_inferred_op + "s" ;
    else if ( isStringNum(arg1) || isStringNum(arg2) )
            return type_inferred_op + "n" ;
    if ( is_enclosed_by_delimiters( arg1, '[', ']' )  ) {
        string arg_first =  trimWS_pair( singleSplit( readstringbetween( arg1, '[', ']') , ",")).first ;
        return type_inferred_op + ( isStringNum(arg_first) ? "n" : "s" ); 
    }
    else if ( is_enclosed_by_delimiters( arg2, '[', ']' ) ) {
        string arg_first =  trimWS_pair( singleSplit( readstringbetween( arg2, '[', ']') , ",")).first ;
        return type_inferred_op + ( isStringNum(arg_first) ? "n" : "s" ); 
    }        
    else if ( ind1 >= arg1.size() && word1.find(".", 0) != std::string::npos ) {
        long index = word1.find(".", 0) + 1;
        string temp = readstringbetween(word1.substr(index) , '<' , '>' );
        if (temp == "\"Id\"" || temp == "\"Kids\"")
            return type_inferred_op + "n";
        else if (temp == "\"href\"" )
            return type_inferred_op + "s";     
    } 
    else if ( ind2 >= arg2.size() && word2.find(".", 0) != std::string::npos ) {
        long index = word2.find(".", 0) + 1;
        string temp = readstringbetween(word2.substr(index) , '<' , '>' );
        if (temp == "\"Id\"" || temp == "\"Kids\"")
            return type_inferred_op + "n";
        else if (temp == "\"href\"" )
            return type_inferred_op + "s";        
    } 
    else  
        abort_with_error_message("Can't infer type of operator!\n"); // Check for 

    return op;
}


string read_next_word (string s, long start_pos, long& end_pos) {
    long i = next_non_WS_char(s, start_pos);
    if (i >= s.length()) {
        end_pos = s.length();
        return "";
    }
    long j = i;
    long ob = 0;
    long sb = 0;
    bool is_string = false;

    if (s[j] == '(') 
        j++, ob++;
    while ( j < s.size() && !(isWS(s[j]) && !is_string && ob == 0 && sb == 0) && !(s[j] == '(' && !is_string && ob == 0 && sb == 0)  ) {  // The open brace case is to treat arguments to a prefix operator as a new word
        if (s[j] == '(')
            ob++;
        else if (s[j] == ')')
            ob--;
        else if (s[j] == '[')
            sb++;
        else if (s[j] == ']')
            sb--;
        else if (s[j] == '\"' && s[j-1] != '\\')
            is_string = !is_string;
        j++;
    }
    end_pos = j;
    return s.substr(i, j - i);
}


long next_non_WS_char (string s, long start_pos) {
    long i = start_pos;
    while ( i < s.length() ) {
        if ( !isWS( s[i]) )
            return i;
        i++;
    }
    return i;
}


string replace_substring (string s, string old_substring, string new_substring) {
    size_t init_pos = s.find(old_substring, 0);
    size_t final_pos = init_pos + old_substring.size();
    string prefix_string = s.substr(0, init_pos);
    string suffix_string = s.substr(final_pos, s.size() - final_pos );
    return (prefix_string + new_substring + suffix_string);
}



