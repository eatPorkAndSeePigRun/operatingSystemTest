int factorial(int n);
int main(int argc, char **argy){
    int n;
    if(argc < 2){
        printf("Usage:%s n\n", argy[0]);
        return -1;
    }
    else {
        n = atoi(argy[1]);
        printf("Factorial of %d is %d.\n", n, factorial(n));
    }
    return 0;
}

