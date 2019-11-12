CC=g++ -std=c++14

all: test

test: test.o BigInt.o
	$(CC) -o test test.o BigInt.o; ./test
	
BigInt.o: BigInt.cpp
	$(CC) BigInt.cpp -c
	
clean:
	-rm test test.o BigInt.o
