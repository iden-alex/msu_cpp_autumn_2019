CC=g++ -std=c++14

all: test

test: test.o Serializer.h
	$(CC) -o test test.cpp; ./test

clean:
	-rm test
