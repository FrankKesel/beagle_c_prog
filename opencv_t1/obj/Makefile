#Makefile for opencv_t1 project

#Compiler and flags
CC      = g++
CFLAGS  = -c -g3 -o0 -Wall $(shell pkg-config --cflags opencv)
LDFLAGS = $(shell pkg-config --libs opencv)

#Name of executable
all: t1

#Link 
t1: boneCV.o
	$(CC) $(LDFLAGS) $< -o $@

#Compile
boneCV.o: ../src/boneCV.cpp
	$(CC) $(CFLAGS) $< -o $@


clean:
	rm -rf *.o t1
