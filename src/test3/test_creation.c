#include <stdio.h>

int main () {
    int p1, p2;
    p1 = fork();
    //while (p1 == fork() == -1);
    if (p1 == 0) {
        putchar('y');
    } else {
        p2 = fork();
        //while (p2 == fork() == -1);
        if (p2 == 0) {
	    putchar('z');
	} else {
	    putchar('x');
        }
    }
    return 0;
}
