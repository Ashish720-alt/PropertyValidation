#include<iostream>
#include<set>
#include<vector>
#include<map>
#include<string>
#include<fstream>
#include<stdlib.h>
#include "AGspec.hpp"
#include "AGmacros.hpp"
#include "AG_resolveattributes.hpp"
#include "../macros.hpp"
#include "../utility.hpp"
#include "../printFunctions.hpp"
#include "AGutility.hpp"

using namespace std;

void AGSpec ( fstream& spec_file ) {
    string single_spec = read_AG_property_without_delimiters(spec_file);  
    find_attribute( single_spec );


    return;
}






