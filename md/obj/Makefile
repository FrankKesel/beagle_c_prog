#Makefile for motion detect (md) project

#Compiler and flags
CC      = g++
CFLAGS  = -c -g3 -o0 -Wall $(shell pkg-config --cflags opencv)
LDFLAGS = $(shell pkg-config --libs opencv)

#Name of executable
all: md

#Link 
md: main.o
	$(CC) $(LDFLAGS) $< -o $@

#Compile
main.o: ../src/main.cpp
	$(CC) $(CFLAGS) $< -o $@


clean:
	rm -rf *.o md
