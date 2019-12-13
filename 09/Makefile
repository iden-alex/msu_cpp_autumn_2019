CC=g++ -std=c++14


all: test

run: file_sort.o
	$(CC) -o file_sort file_sort.cpp -lpthread
test: file_sort.o check_sort.o
	$(CC)  -o file_sort file_sort.cpp -lpthread; ./file_sort "test0.bin"; 
	$(CC) -o check_sort check_sort.cpp; ./check_sort

clean:
	-rm file_sort file_sort.o check_sort check_sort.o

