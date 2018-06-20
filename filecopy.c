#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/limits.h>
//You may also add more include directives as well.

//THIS VALUE CANNOT BE CHANGED.
//You should use this value when creating your buffer.
// And also to limit the amount each read CAN do.
#define BUFF_MAX 13
#define USER_NAME "Ilya Kopyl"


void check_for_errors(ssize_t, const char*);
int open_to_read(const char *);
int open_to_write(const char *);
size_t copy_file_contents(int, int);

int main(int argc, char const *argv[])
{
    char *file1_path = (char *) malloc(PATH_MAX);
    char *file2_path = (char *) malloc(PATH_MAX);

	printf("Welcome to the File Copy Program by %s!\n", USER_NAME);
	printf("Enter the name of the file to copy from:\n");
    int bytes_read = scanf("%s", file1_path);
    if (bytes_read <= 0)
        perror("Unable to scan the input...");


	printf("Enter the name of the file to copy to:\n");
	bytes_read = scanf("%s", file2_path);
	if (bytes_read <= 0)
	    perror("Unable to scan the input...");

    int fd1 = open_to_read(file1_path);
    free(file1_path);

    int fd2 = open_to_write(file2_path);
    free(file2_path);

    size_t bytes_copied = copy_file_contents(fd1, fd2);

    int close_status = close(fd2);
    check_for_errors(close_status, "File close error...");

    close_status = close(fd1);
    check_for_errors(close_status, "File close error...");

	printf("File Copy Successful, %lu bytes copied\n", bytes_copied);

	return 0;
}

void check_for_errors(ssize_t signal_code, const char* error_message)
{
    if (signal_code < 0)
        perror(error_message);
}

int open_to_read(const char *path)
{
    int fd = open(path, O_RDONLY);
    check_for_errors(fd, "File open error...");

    return fd;
}

int open_to_write(const char *path)
{
    int fd = open(path, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);        // 644
    check_for_errors(fd, "File open error...");

    return fd;
}

size_t copy_file_contents(const int source_fd, const int target_fd)
{
    ssize_t bytes_read = 0;
    ssize_t bytes_written = 0;
    size_t bytes_copied_total = 0;
    char *buf = (char *) malloc(BUFF_MAX);

    // relying on the fact that EOF is equivalent to FALSE
    while ((bytes_read = read(source_fd, buf, BUFF_MAX))) {
        bytes_written = write(target_fd, buf, (size_t) bytes_read);
        check_for_errors(bytes_written, "write error...");
        bytes_copied_total += bytes_written;
    }
    check_for_errors(bytes_read, "read error...");

    free(buf);

    return bytes_copied_total;
}