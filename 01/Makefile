CC=g++

all: calc

run: calc
	calc

test: calc
	python3 -u ./test.py
	
calc: calc.cpp
	g++ -o calc calc.cpp
