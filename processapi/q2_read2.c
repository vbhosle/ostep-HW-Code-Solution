#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include<fcntl.h>
#include<string.h>
// Read with different file descriptor in parent and child
int main(int argc, char *argv[]) {
    printf("hello world (pid:%d) \n", (int) getpid());

    int fd = open("file.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
    write(fd, "Writing before fork", strlen("Writing before fork \n"));
    printf("Fd in write mode %d \n", fd);
    // lseek(fd, 0, SEEK_SET); //go back to byte 0 for upcoming read
    int rc = fork();

    if(rc < 0) {
        fprintf(stderr, "fork failed \n");
        close(fd);
        exit(1);
    }
    else if (rc == 0) {
        char buf[20];
        int readFd = open("file.txt", O_RDONLY);
        int readReturn = read(readFd, buf, 20);
        printf("Read from child (FD %d) (readReturn %d) - %s \n", readFd, readReturn, buf);
        
        printf("hello, I am child (pid:%d)\n", (int) getpid());
        close(readFd);
    }
    else {
        char buf[20];
        int readFd = open("file.txt", O_RDONLY);
        int readReturn = read(readFd, buf, 20);
        printf("Read from parent (FD %d) (readReturn %d) - %s \n", readFd, readReturn, buf);

        int wc = wait(NULL); //note that reads are parallel, before calling wait in parent
        printf("hello, I am parent of %d (wc:%d) (pid: %d)\n", rc, wc, (int) getpid());
        close(readFd);
        close(fd);
    }

    return 0;
}

/* Output
hello world (pid:677) 
Fd in write mode 3
Read from parent (FD 4) (readReturn 20) - Writing before fork
Read from child (FD 4) (readReturn 20) - Writing before fork
hello, I am child (pid:678)
hello, I am parent of 678 (wc:678) (pid: 677)
*/
// Conclusion - Though file descriptor number is same in child and parent somehow reading position for both start from beginning and both can read without lseek