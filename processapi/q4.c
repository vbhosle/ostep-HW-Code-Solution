#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    printf("hello world (pid:%d)\n", (int) getpid());
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        printf("hello, I am child (pid:%d)\n", (int) getpid());

        //execvp takes arguments as array, take program name instead of entire path
        /*
        char *myargs[2];
        myargs[0] = ".";  
        myargs[1] = NULL; // marks end of array
        execvp("ls", myargs);  // runs ls
        */

        //execvp - takes full path of the file
        /*
        int exeResult = execl("/bin/ls", ".", NULL);  // runs ls
        printf("executed command %d \n", exeResult);
        if(exeResult == -1) {
            perror("command execution failed. \n");
        }
        printf("this shouldn't print out \n");
        */

        //execle
        /*
        char* env_list[] = { "PLANET=MARS",
                    "DEVELOPER=VIRAJ",
                    "lines=65",
                    NULL
                    };

        //Command line expansions are provided by shell before the command is called
        int exeResult = execle("/bin/bash","bash", "-c", "echo \"$DEVELOPER is from $PLANET\"", NULL, env_list);  // Access environment variables
        printf("executed command %d \n", exeResult);
        if(exeResult == -1) {
            perror("command execution failed. \n");
        }
        printf("this shouldn't print out \n");
        */

        //execlp - version of execl that takes filename instead of whole path
        /*
        int exeResult = execlp("ls", ".", NULL);
        printf("executed command %d \n", exeResult);
        if(exeResult == -1) {
            perror("command execution failed. \n");
        }
        printf("this shouldn't print out \n");
        */

        //execv
        /*
        char *myargs[2];
        myargs[0] = ".";  
        myargs[1] = NULL; // marks end of array
        int exeResult = execv("/bin/ls", myargs);
        printf("executed command %d \n", exeResult);
        if(exeResult == -1) {
            perror("command execution failed. \n");
        }
        printf("this shouldn't print out \n");
        */

        //execvp
        char *myargs[2];
        myargs[0] = ".";  
        myargs[1] = NULL; // marks end of array
        int exeResult = execvp("ls", myargs); //only file name is enough no path required
        printf("executed command %d \n", exeResult);
        if(exeResult == -1) {
            perror("command execution failed. \n");
        }
        printf("this shouldn't print out \n");
        
    } else {
        // parent goes down this path (original process)
        int wc = wait(NULL);
        printf("hello, I am parent of %d (wc:%d) (pid:%d)\n",
	       rc, wc, (int) getpid());
    }
    return 0;
}

/*Output
*/