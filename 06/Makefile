CC=g++ -std=c++14

all: test

test: test.o format.h
	$(CC) -o test test.cpp; ./test

clean:
	-rm test test.o
 
