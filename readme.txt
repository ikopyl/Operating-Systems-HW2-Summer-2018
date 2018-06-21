Build Instructions:
$ make
gcc -I -Wall filecopy.c -o fc


Run Instructions:
$ ./fc
together with strace:
$ strace -c ./fc


strace output:

$ strace -c ./fc 
Welcome to the File Copy Program by Ilya Kopyl!
Enter the name of the file to copy from:
/home/csc415/depot/aaa.txt
Enter the name of the file to copy to:
/home/csc415/depot/bbb.txt
File Copy Successful, 246 bytes copied
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
  0.00    0.000000           0        23           read
  0.00    0.000000           0        23           write
  0.00    0.000000           0         4           open
  0.00    0.000000           0         4           close
  0.00    0.000000           0         4           fstat
  0.00    0.000000           0         1         1 lseek
  0.00    0.000000           0         7           mmap
  0.00    0.000000           0         4           mprotect
  0.00    0.000000           0         1           munmap
  0.00    0.000000           0         3           brk
  0.00    0.000000           0         5         3 access
  0.00    0.000000           0         1           execve
  0.00    0.000000           0         1           arch_prctl
------ ----------- ----------- --------- --------- ----------------
100.00    0.000000                    81         4 total




As you can see, there are 23 read() and write() system calls.  

However, if I comment out everything inside main(), recompile and run strace again, I would get the following:

$ strace -c ./fc 
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
  0.00    0.000000           0         1           read
  0.00    0.000000           0         2           open
  0.00    0.000000           0         2           close
  0.00    0.000000           0         2           fstat
  0.00    0.000000           0         7           mmap
  0.00    0.000000           0         4           mprotect
  0.00    0.000000           0         1           munmap
  0.00    0.000000           0         1           brk
  0.00    0.000000           0         3         3 access
  0.00    0.000000           0         1           execve
  0.00    0.000000           0         1           arch_prctl
------ ----------- ----------- --------- --------- ----------------
100.00    0.000000                    25         3 total


– and if I go ahead and calculate the delta between values in these two tables, I would get the actual numbers of system calls that was done by my code:

% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
  0.00    0.000000           0        22           read
  0.00    0.000000           0        23           write
  0.00    0.000000           0         2           open
  0.00    0.000000           0         2           close
  0.00    0.000000           0         2           fstat
  0.00    0.000000           0         0           mmap
  0.00    0.000000           0         0           mprotect
  0.00    0.000000           0         0           munmap
  0.00    0.000000           0         2           brk
  0.00    0.000000           0         2         3 access
  0.00    0.000000           0         0           execve
  0.00    0.000000           0         0           arch_prctl
------ ----------- ----------- --------- --------- ----------------
100.00    0.000000                    55         3 total


If the size of the file is 246 bytes, and the buffer size is 13 bytes, then it leaves us 19 write() system calls just to copy the file. However, there are 20 read() system calls that occur in my code – the last one reads EOF, which terminates while loop (see the line #90).

Yet, this still leaves us 2 more read() calls and 4 more write() calls. Since I'm using printf() to print 4 messages to stdout, that takes exactly 4 remaining write() calls. As for read() system call, the remaining 2 calls are invoked by scanf() function.

Since I work on 2 files, it is not a surprise that there are 2 open() and 2 close() system calls. Similarly, I use 2 access() calls to verify that the 1st file exists and is readable.



Verification that the files are copied precisely:

$ diff -s aaa.txt bbb.txt 
Files aaa.txt and bbb.txt are identical



Size of the files (246 bytes) matches the number of bytes copied by my application:

$ ls -la
total 28K
drwxrwxr-x  5 csc415 csc415 4.0K Jun 20 16:03 .
drwxr-xr-x 34 csc415 csc415 4.0K Jun 20 21:26 ..
-rw-rw-r--  1 csc415 csc415  246 Jun 20 16:02 aaa.txt
-rw-r--r--  1 csc415 csc415  246 Jun 20 22:03 bbb.txt


Another example:
$ ./fc 
Welcome to the File Copy Program by Ilya Kopyl!
Enter the name of the file to copy from:
/home/csc415/depot/415_HW2.pdf
Enter the name of the file to copy to:
/home/csc415/depot/copy.pdf
File Copy Successful, 368996 bytes copied

$ pwd
/home/csc415/depot/csc415-p2-ikopyl

$ diff -s ../415_HW2.pdf ../copy.pdf 
Files ../415_HW2.pdf and ../copy.pdf are identical

