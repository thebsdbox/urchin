#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

int forkSelf (int zombieCount)
{
    pid_t child_pid;
    for (int i = 0; i < zombieCount; i++) {
        
        child_pid = fork();
        if (child_pid > 0) {
            // parent process will sleep for 30 seconds and exit, without a call to wait()
            fprintf(stderr,"parent process - %d\n", getpid());
            //sleep(1);
            //exit(0);
        }
        else if (child_pid == 0) {
            // Child process is killed immediately, but the parent never cleans up! ZOMBIE TIME
            fprintf(stderr,"child process - %d\n", getpid());
            signal(SIGHUP, SIG_IGN);
            close(0);
            close(1);
            close(2);
            //
            chdir("/");
            //
            //
            setsid();
            //
            //while(1) {
            //    sleep(1);
            //}
            
            //            setsid();
            //
            exit(0);
        }
        else if (child_pid == -1) {
            // fork() error
            perror("fork() call failed");
            exit (-1);
        }
        else {
            // this should not happen
            fprintf(stderr, "unknown return value of %d from fork() call", child_pid);
            exit (-2);
        }
    }
    return 0;
}

int becomeParent() {

        pid_t pid;
        
        if (( pid = fork()) < 0) exit(0);
        else if(pid != 0) exit(0); // kill parent process
        
        fprintf(stderr,"child running...pid=%d\n",getpid());
        signal(SIGHUP, SIG_IGN);
        close(0);
        close(1);
        close(2);
        //
        chdir("/");
        //
        //
        setsid();
        //
        while(1) {
            sleep(1);
        }
    }

