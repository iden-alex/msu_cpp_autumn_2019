CC=g++ -std=c++14

all: test

test: test.o pool.h
	$(CC) -o test test.cpp -lpthread; ./test

clean:
	-rm test test.o
