CC=g++

all: test

test: LinearAllocator.cpp test1 test2 test3
	python3 -u ./test.py

test1: test1.cpp
	g++ -std=c++14 -o test1 test1.cpp
	
test2: test2.cpp
	g++ -std=c++14 -o test2 test2.cpp

test3: test3.cpp
	g++ -std=c++14 -o test3 test3.cpp
