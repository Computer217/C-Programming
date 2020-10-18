#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h> 
#include <unistd.h>

#define MAX_FD      20

void die(char *s)
{
    fprintf(stderr, "Error: %s\n", s);
    if (errno)
        perror("errno");
    exit(EXIT_FAILURE);
} 

int main(int argc, char **argv)
{
    // check FDs from 0 to MAX_FD 
    char    line[512]; // should be enough for 20 integers 
    char    *p;

    p = line;
    for (int fd = 0; fd <= MAX_FD; fd ++) {
        // if an FD is valid, print it
        if (fcntl(fd, F_GETFD) != -1 || errno != EBADF) {
            int  len;
            len = snprintf(p, 10, "%d ", fd);  
            p += len;
        }
    }
    // print everything in one fprintf()
    fprintf(stderr, "checkof: %s\n", line);

    // copy from stdin or a file to stdout
    int ch;

    // example of redirect stdin, for this process
    if (argc == 2) {
        int fd;

        // open file for read only
        fd = open(argv[1], O_RDONLY);
        if (fd < 0)
            die("open() failed.");
        dup2(fd, 0);
        close(fd);
    }

    while ((ch = getchar()) != EOF) 
        putchar(ch);

    return 0;
}
