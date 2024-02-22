Summation

Implement an assembly language function that can be called from C, with the following declaration:

void sum(int64_t *x, size_t n);

The function takes two arguments: a pointer x to a non-empty array of 64-bit integers in two's complement form and the size n of this array. The operation of the function is described by the following pseudocode:

y = 0; 
for (i = 0; i < n; ++i) 
    y += x[i] * (2 ** floor(64 * i * i / n)); 
x[0, ..., n-1] = y;

In this pseudocode, ** represents exponentiation, and y is a (64 * n)-bit number in two's complement form. The function should store the result in the array x using little-endian order. It is meant to perform the calculation "in place," using only the memory pointed to by x and not requiring additional memory.

It is assumed that the x pointer is valid and that n is a positive value less than 2 raised to the power of 29.

Compilation

The solution will be compiled with the command:

nasm -f elf64 -w+all -w+error -o sum.o sum.asm

Usage Example

An example of usage is provided in the attached file sum_example.c. It can be compiled and linked with the solution using the commands:

gcc -c -Wall -Wextra -std=c17 -O2 -o sum_example.o sum_example.c
gcc -z noexecstack -o sum_example sum_example.o sum.o