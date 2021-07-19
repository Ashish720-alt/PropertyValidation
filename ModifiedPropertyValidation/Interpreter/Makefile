CC=g++
CPPFLAGS= -g -std=c++11

DEPS= printFunctions.hpp macros.hpp utility.hpp surface_to_internal.hpp ./getGraph/Keys.hpp  ./getGraph/GraphPropertySpec.hpp ./getGraph/IRGraphTraversal.hpp 
DEPS_GRAPHSPEC= ./graphProperty/graphmacros.hpp  ./graphProperty/graphPropertySpec.hpp ./graphProperty/resolveProperty.hpp ./graphProperty/resolveProperty_functions.hpp  ./graphProperty/graphpropertyutility.hpp
DEPS_AP= ./AG/AGmacros.hpp ./AG/AG_resolveattributes.hpp ./AG/AGspec.hpp ./AG/AGutility.hpp ./AG/AG_resolveattributes_functions.hpp

OBJ= interpreter.o printFunctions.o utility.o surface_to_internal.o ./getGraph/Keys.o ./getGraph/GraphPropertySpec.o ./getGraph/IRGraphTraversal.o 
OBJ_GRAPHSPEC= ./graphProperty/graphPropertySpec.o ./graphProperty/resolveProperty.o  ./graphProperty/resolveProperty_functions.o  ./graphProperty/graphpropertyutility.o
OBJ_AP= ./AG/AG_resolveattributes.cpp ./AG/AGspec.cpp ./AG/AGutility.cpp ./AG/AG_resolveattributes_functions.cpp

SEMANTIC_CONDITION_CPP=  ./SemanticCondition/graphpropertyutility1.cpp ./SemanticCondition/resolveSemanticCondition1.cpp  ./SemanticCondition/resolveSemanticCondition_functions1.cpp

SEMANTIC_CONDITION_HPP= ./SemanticCondition/graphpropertyutility1.hpp ./SemanticCondition/graphmacros1.hpp    ./SemanticCondition/resolveSemanticCondition1.hpp  ./SemanticCondition/resolveSemanticCondition_functions1.hpp 


interpreter: $(OBJ) $(OBJ_GRAPHSPEC) $(OBJ_AP) $(SEMANTIC_CONDITION_CPP)
	$(CC) $(CPPFLAGS) -o $@ $^ 

%.o: %.cpp $(DEPS) $(DEPS_GRAPHSPEC) $(DEPS_AP) $(SEMANTIC_CONDITION_HPP)
	$(CC) $(CPPFLAGS) -c -o $@ $< 

clean:
	rm -f *.o
	rm -f ./getGraph/*.o
	rm -f ./graphProperty/*.o

#  
