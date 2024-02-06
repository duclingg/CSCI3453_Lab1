# READ ME

**Name:** Justin Hoang  
**Student ID:** 110069730  
**Class:** CSCI 3453  
**Lab:** 1  
**Due Date:** Febuary 14, 2024  

## Description of Program
A C program using Pthreads to perform matrix multiplication in a multithreaded fashion. Utilizes a dedicated worker thread for each row of the resulting matrix rather than assigning a thread for individual elements. Avoids statically initializing the contents of matrices A and B; instead, read data from an input file (refer to notes below).  
  
Ensures the capability to handle multiple sets of input matrices by continuously reading data in the specified format (details provided below). If any character other than an integer, including negative numbers, is encountered, displays an error message and terminates the program.  
  
Labeled each processed set of data to indicate the corresponding output (e.g., Matrix A, Matrix B, etc.). The program should accommodate matrices with a maximum size of 10000x10000.  

## Source Files


## How to build and run program

