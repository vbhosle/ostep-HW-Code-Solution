#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    printf("hello world (pid:%d)\n", (int) getpid());
    int x = 100;

    int rc = fork();

    if(rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0) {
        printf("hello, I am child (pid:%d), x = %d \n", (int) getpid(), x);
        x = 10000;
        printf("hello, I am child (pid:%d), x = %d \n", (int) getpid(), x);
    }
    else {
        int wc = wait(NULL);
        printf("hello, I am parent of %d (wc:%d) (pid: %d), x = %d \n", rc, wc, (int) getpid(), x);
        x = 200;
        printf("hello, I am parent of %d (wc:%d) (pid: %d), x = %d \n", rc, wc, (int) getpid(), x);
    }

    return 0;
}

/* Output
hello world (pid:404)
hello, I am parent of 405 (pid: 404), x = 100 
hello, I am parent of 405 (pid: 404), x = 200 
hello, I am child (pid:405), x = 100 
hello, I am child (pid:405), x = 10000 
*/
// Conclusion - Child gets it's own copy