#include <stdio.h>
#include <unistd.h>

void main (int argc, char *argv[]) {
    int pid;
    pid = fork();   /* fork child process */
    if (pid < 0) {
        fprintf(stderr, "Fork Failed\n"); 
        exit(-1);
    } else if (pid == 0) {
        execlp("/bin/ls", "ls", NULL);
    } else {    /* child process */
        wait(NULL);
        printf("child Complete\n");
        exit(0);
    }
    /* parent process */
}
