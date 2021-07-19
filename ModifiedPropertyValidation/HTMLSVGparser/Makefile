CC=g++
CFLAGS= -g
DEPS = macros.hpp utility.hpp
OBJ = utility.o parser.o
OBJ2 = utility.o haveopentags.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

parser: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ 

checkopentags: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ 

clean:
	rm -f *.o

