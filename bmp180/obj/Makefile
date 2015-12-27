#Makefile for bmp180 project

#Compiler and flags
CC      = gcc
CFLAGS  = -c -Wall 
LDFLAGS = 

#Name of executable
EXE=bmp180
#Objectfiles
OBJ=main.o i2c.o bmp180.o

all: $(EXE)

#Link 
$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $(EXE)

#Compile
main.o: ../src/main.c
	$(CC) $(CFLAGS) $< -o $@

i2c.o: ../src/i2c.c ../src/i2c.h
	$(CC) $(CFLAGS) $< -o $@

bmp180.o: ../src/bmp180.c ../src/bmp180.h
	$(CC) $(CFLAGS) $< -o $@


clean:
	rm -rf *.o t1
