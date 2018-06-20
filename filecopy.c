#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//You may also add more include directives as well.

//THIS VALUE CANNOT BE CHANGED.
//You should use this value when creating your buffer.
// And also to limit the amount each read CAN do.
#define BUFF_MAX 13
#define USER_NAME "Ilya Kopyl"


void check_for_errors(int signal_code, const char*);

int main(int argc, char const *argv[])
{
	printf("Welcome to the File Copy Program by %s!\n", USER_NAME);
	printf("Enter the name of the file to copy from:\n");

	// TO DO

	printf("Enter the name of the file to copy to:\n");

	// TO DO


    char* file1_path = "/Users/ilya.kopyl/depot/git_home/csc415/homework_assignments/hw2/aaa.txt";
    int fd1 = open(file1_path, O_RDONLY);
    check_for_errors(fd1, "File open error...");

    char* file2_path = "/Users/ilya.kopyl/depot/git_home/csc415/homework_assignments/hw2/bbb.txt";
    int fd2 = open(file2_path, O_CREAT|O_WRONLY|O_TRUNC, 640);
    check_for_errors(fd2, "File open error...");


    int close_status = close(fd2);
    check_for_errors(close_status, "File close error...");

    close_status = close(fd1);
    check_for_errors(close_status, "File close error...");




	size_t bytes_copied = 0;

	printf("File Copy Successful, %lu bytes copied\n", bytes_copied);

	return 0;
}

void check_for_errors(int signal_code, const char* error_message)
{
    if (signal_code < 0)
        perror(error_message);
}