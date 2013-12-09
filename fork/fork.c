#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main() {
        printf("main");
        pid_t pid;
        pid = fork();
        // son process create here, exact copy from parent process
        // son process will execute the main funciton after parent process
        if(pid<0) {
                printf("return");
                return 0;
        }
        if(pid==0) {
                printf("I am the child, my pid is %d\n", getpid());
        }else {
                printf("I am the parent, my pid is %d, my child pid is%d\n", getpid(), pid);
        }
//      sleep(6);
}

