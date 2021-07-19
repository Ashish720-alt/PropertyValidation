CC=g++
CPPFLAGS= -g -std=c++11
DEPS= surface_to_internal.hpp macros.hpp utility.hpp printFunctions.hpp
OBJ= main.o surface_to_internal.o utility.o printFunctions.o
specConverter: $(OBJ) 
	$(CC) $(CPPFLAGS) -o $@ $^ 

%.o: %.cpp $(DEPS) 
	$(CC) $(CPPFLAGS) -c -o $@ $< 

clean:
	rm -f *.o

