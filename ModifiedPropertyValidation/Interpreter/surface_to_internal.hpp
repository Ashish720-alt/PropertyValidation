#ifndef SURFACETOINTERNAL_HPP
#define SURFACETOINTERNAL_HPP

using namespace std;

std::string surface_to_internal_spec (std::string surface_spec_filename);

std::string spec_file_changes( std::string s);

std::string add_end_delimiter (std::string s, std::string new_section_string) ;

std::string add_second_period (std::string s) ;

std::string modify_forall_delimiter (std::string s);

std::string trivial_modifications (std::string s);

std::string add_id_fields_in_quantifiers ( string P) ;

std::string replace_all_substr ( std::string s, std::string initial_substr, std::string final_substr);

std::string parse_Spec (std::string s);

void initialize_operators( std::map<string, long>& infix_operator_precedence, std::set<string>& prefix_operators);

string infix_to_prefix (string s);

string type_inference (string op, string arg1, string arg2) ;


string replace_substring (string s, string old_substring, string new_substring) ;

string read_next_word (string s, long start_pos, long& end_pos)  ;

long next_non_WS_char (string s, long start_pos);

std::string remove_unnecessary_brackets(std::string s);

#endif