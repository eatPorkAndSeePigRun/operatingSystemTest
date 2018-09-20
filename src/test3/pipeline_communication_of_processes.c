#include <stdio.h>
#include <unistd.h>

char parent[] = "A message from parent.";
char child[] = "A message from child.";

void errexit(const char *s) {
    printf(s);
    exit(1);
}

int main () {
    int chan1[2], chan2[2];
    char buf[100];
    if (-1 == pipe(chan1) || -1 == pipe(chan2) ) {
        errexit("pipe\n");
    }
    if (fork()) {
        close(chan1[0]);
        close(chan2[1]);
        write(chan1[1], parent, sizeof(parent));
        close(chan1[1]);
        read(chan2[0], buf, 100);
        printf("parent process: %s \n", buf);
        close(chan2[0]);
    } else {
        close(chan1[1]);
        close(chan2[0]);
        read(chan1[0], buf, 100);
        printf("child process: %s \n", buf);
        write(chan2[1], child, sizeof(child));
        close(chan2[1]);
        close(chan1[0]);
    }
    return 0;
}
