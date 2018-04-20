
#include<stdio.h>
#include<signal.h>
#include<unistd.h>

void sig_handler(int signo)
{
    if (signo == SIGUSR1)
        printf("received SIGUSR1\n");
    else if (signo == SIGKILL)
        printf("received SIGKILL\n");
    else if (signo == SIGSTOP)
        printf("received SIGSTOP\n");
    else if (signo == SIGTERM)
        printf("received SIGTERM\n");
    else if (signo == SIGINT)
        printf("received SIGINT\n");
}

void setSignalHander() {
    printf("Setting up Signal handlers to capture kill signals");
    if (signal(SIGUSR1, sig_handler) == SIG_ERR)
        printf("can't catch SIGUSR1\n");
    if (signal(SIGKILL, sig_handler) == SIG_ERR)
        printf("can't catch SIGKILL\n");
    if (signal(SIGSTOP, sig_handler) == SIG_ERR)
        printf("can't catch SIGSTOP\n");
    if (signal(SIGTERM, sig_handler) == SIG_ERR)
        printf("can't catch SIGTERM\n");
    if (signal(SIGINT, sig_handler) == SIG_ERR)
        printf("can't catch SIGINT\n");
}
