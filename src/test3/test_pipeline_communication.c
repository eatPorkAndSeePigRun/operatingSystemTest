#include <unistd.h>
#include <stdio.h>

int main() {
    int I, r, p1, p2, p3, fd[2];
    char Outpipe[50], Inpipe[5];
    pipe(fd);
    p1 = fork();
    //while (p1 == fork());
    if (p1 == 0) {
        lockf(fd[1], 1, 0);
        sprintf(Outpipe, "child process P1 is sending message!\n");
        printf("child process P1!\n");
        write(fd[1], Outpipe, 50);
        sleep(5);
        lockf(fd[1], 0, 0);
        exit(0);
    } else {
        p2 = fork();
        // while(p2 == fork() == -1);
        if (p2 == 0) {
            lockf(fd[1], 1, 0);
            sprintf(Outpipe, "child process P2 is sending message!\n");
            printf("child process P2!\n");
            write(fd[1], Outpipe, 50);
            sleep(5);
            lockf(fd[1], 0, 0);
            exit(0);
        } else {
            p3 = fork();
            // while(p3 == fork() == -1);
            if (p3 == 0) {
                lockf(fd[1], 1, 0);
                sprintf(Outpipe, "child process P3 is sending message!\n");
                printf("child process P3!\n");
                write(fd[1], Outpipe, 50);
                sleep(5);
                lockf(fd[1], 0, 0);
                exit(0); 
            }
        }
        wait(0);
        if (r = read(fd[0], Inpipe, 50) == -1) {
            printf("can't read pipe\n");
        } else {
            printf("%s\n", Inpipe);
        }
        wait(0);
        if (r = read(fd[0], Inpipe, 50) == -1) {
           printf("can't read pipe\n"); 
        } else {
            printf("%s\n", Inpipe);
        }
        exit(0);
    } 
    return 0;
}
