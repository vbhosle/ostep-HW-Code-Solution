#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include<fcntl.h>
#include<string.h>
// Read with different file descriptor in parent and child
int main(int argc, char *argv[]) {
    printf("hello world (pid:%d)\n", (int) getpid());

    int fd = open("file.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
    write(fd, "Writing before fork. ", strlen("Writing before fork. "));
    // lseek(fd, 0, SEEK_SET); //go back to byte 0 for upcoming read
    int rc = fork();

    if(rc < 0) {
        fprintf(stderr, "fork failed\n");
        close(fd);
        exit(1);
    }
    else if (rc == 0) {
        write(fd, "Child writing1. ", strlen("Child writing1. "));
        write(fd, "Child writing2. ", strlen("Child writing2. "));
        write(fd, "Child writing3. ", strlen("Child writing3. "));
        write(fd, "Child writing4. ", strlen("Child writing4. "));
        write(fd, "Child writing5. ", strlen("Child writing5. "));
        printf("hello, I am child (pid:%d), fd = %d \n", (int) getpid(), fd);
    }
    else {
        write(fd, "Parent writing1. ", strlen("Parent writing1. "));
        write(fd, "Parent writing2. ", strlen("Parent writing2. "));
        write(fd, "Parent writing3. ", strlen("Parent writing3. "));
        write(fd, "Parent writing4. ", strlen("Parent writing4. "));
        write(fd, "Parent writing5. ", strlen("Parent writing5. "));
        int wc = wait(NULL); //note that reads are parallel, before calling wait in parent
        printf("hello, I am parent of %d (wc:%d) (pid: %d), fd = %d \n", rc, wc, (int) getpid(), fd);
        char buf[500];
        lseek(fd, 0, SEEK_SET);
        int readReturn = read(fd, buf, 500);
        printf("File content - %s", buf);
        close(fd);
    }

    return 0;
}

/* Output
viraj@DESKTOP-92P0JKD:/mnt/e/repos/ostep-HW/processapi$ ./q2_write
hello world (pid:922)
hello, I am child (pid:923), fd = 3
hello, I am parent of 923 (wc:923) (pid: 922), fd = 3
File content - Writing before fork. Parent writing1. Parent writing2. Child writing1. Child writing2. Parent writing3. Parent writing4. Child writing3. Child writing4. Parent writing5. Child writing5.
*/
// Conclusion - Write seem to block other writes till completion. Though writes are alternate output is not messed up.