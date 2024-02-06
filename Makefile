# Makefile for 'main' program

# Our output program name
PROG = main

# The compiler
CC = gcc

# Debugging flags
CFLAGS = -lpthread -o

all: main.c
	  $(CC) $(CFLAGS) main.o main.c

# Clean removes build files that are created
clean:
	rm main.o
