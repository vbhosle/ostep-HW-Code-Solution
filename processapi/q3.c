#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void signalHandler(int signo){
    if(signo == SIGINT)
    printf("Signal caught! by pid %d \n", getpid());
    return;
}

int main(int argc, char *argv[]) {
    printf("hello world (pid:%d)\n", (int) getpid());
    int parentId = getpid();
    int rc = fork();

    if(rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0) {
        printf("hello, I am child (pid:%d)\n", (int) getpid());
        kill(parentId, SIGINT);
    }
    else {
        signal(SIGINT, signalHandler);//register handler for interrupt signal
        pause(); //wait for signal
        printf("hello, I am parent of %d (pid: %d) \n", rc, (int) getpid());
    }

    return 0;
}

/* Output
viraj@DESKTOP-92P0JKD:/mnt/e/repos/ostep-HW/processapi$ ./q3
hello world (pid:1119)
hello, I am child (pid:1120)
Signal caught! by pid 1119
hello, I am parent of 1120 (pid: 1119)
*/