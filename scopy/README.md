Special File Copying

Implement an assembly program named scopy that takes two file names as parameters:

./scopy in_file out_file

The program checks the number of parameters.
 If it is not equal to 2, the program exits with code 1.

It attempts to open the in_file for reading. 
If unsuccessful, the program exits with code 1.

It tries to create a new out_file for writing with permissions -rw-r--r--. 
If this fails, for instance, because the file already exists, the program exits with code 1.

The program reads from in_file and writes to out_file. 
If there is a read or write error, the program exits with code 1.

For each byte read from in_file that is the ASCII code of the letter s or S, it writes this byte to out_file.

For each maximum non-empty sequence of bytes read from in_file that does not contain a byte with the ASCII code of the letter s or S, it writes to out_file a 16-bit number containing the count of bytes in this sequence modulo 65536. 
This number is written in binary using little-endian order.

At the end, the program closes the files and, if successful, exits with code 0.
Additional Requirements and Tips

The program should use Linux system functions: sys_open, sys_read, sys_write, sys_close, sys_exit. It should check the success of system function calls (except sys_exit). If a system call fails, the program should exit with code 1. In every situation, before exiting, the program should explicitly call sys_close for each file it has opened.

For satisfactory performance, reads and writes should be buffered. 
Optimal buffer sizes should be selected and mentioned in a comment.
Submitting the Solution

The solution will be compiled with the commands:

nasm -f elf64 -w+all -w+error -o scopy.o scopy.asm ld --fatal-warnings -o scopy scopy.o

An example usage is provided in the attached files example1.in, example1.out. 
The contents of these files can be viewed with the command hexdump -C.