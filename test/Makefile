CC=g++
INC=-I../src
CFLAGS=-c -Wall $(INC)

all: test

test: test.o
	$(CC) output/test.o -o output/test

test.o: test.cpp
	$(CC) $(CFLAGS) test.cpp -o output/test.o

clean:
	rm -rf output/*o output/test