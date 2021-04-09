#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include<fcntl.h>
#include<string.h>
// Read with same file descriptor in parent and child
int main(int argc, char *argv[]) {
    printf("hello world (pid:%d)\n", (int) getpid());

    int fd = open("file.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
    write(fd, "Writing before fork", strlen("Writing before fork\n"));
    lseek(fd, 0, SEEK_SET); //go back to byte 0 for upcoming read
    int rc = fork();

    if(rc < 0) {
        fprintf(stderr, "fork failed\n");
        close(fd);
        exit(1);
    }
    else if (rc == 0) {
        char buf[20];
        int readReturn = read(fd, buf, 20);
        printf("Read from child (readReturn %d) - %s", readReturn, buf);
        lseek(fd, 0, SEEK_SET); //go back to byte 0 for upcoming read
        // write(fd, "Child writing", strlen("Child writing\n"));
        printf("hello, I am child (pid:%d), fd = %d \n", (int) getpid(), fd);
    }
    else {
        char buf[20];
        int readReturn = read(fd, buf, 20);
        printf("Read from parent (readReturn %d) - %s", readReturn, buf);
        lseek(fd, 0, SEEK_SET); //go back to byte 0 for upcoming read
        // write(fd, "Parent writing", strlen("Child writing\n"));
        int wc = wait(NULL); //note that reads are parallel, before calling wait in parent
        printf("hello, I am parent of %d (wc:%d) (pid: %d), fd = %d \n", rc, wc, (int) getpid(), fd);
        close(fd);
    }

    return 0;
}

/* Output
viraj@DESKTOP-92P0JKD:/mnt/e/repos/ostep-HW/processapi$ ./q2
hello world (pid:411)
Read from child (readReturn 0) - hello, I am child (pid:412), fd = 3
Read from parent (readReturn 20) - Writing before forkhello, I am parent of 412 (wc:412) (pid: 411), fd = 3
viraj@DESKTOP-92P0JKD:/mnt/e/repos/ostep-HW/processapi$ ./q2
hello world (pid:413)
Read from child (readReturn 20) - Writing before forkhello, I am child (pid:414), fd = 3 
Read from parent (readReturn 20) - Writing before forkhello, I am parent of 414 (wc:414) (pid: 413), fd = 3
*/
// Conclusion - It is unsafe to read with same file descriptor in parallel, sometimes parent may try to read before lseek is called in child
// Try commenting lseek