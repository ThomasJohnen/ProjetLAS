#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "info.h"
#include "reseau.h"
#include "utils_v2.h"

#define NB_SOCKET 100
#define NB_ADRESSES 2
#define MAX_TAILLE_BUFFER 256

void controllerFils(void* sl, void* nbSockets){
    int* sockfdlist = sl;
    int nbSocket = *(int*) nbSockets;

    struct pollfd fds[NB_SOCKET];
    char commande[MAX_TAILLE_BUFFER];
    int taille;
    
    for (int i = 0; i < nbSocket; i++) {
        fds[i].fd = sockfdlist[i];
        fds[i].events = POLLIN;
    }

    char* espace = "\n";
    while(1) {
        int ret = spoll(fds, nbSocket, 1000);
        if (ret == 0) continue;

        //printf("%d",nbSocket);
        for (int i = 0; i < nbSocket; i++) {
            if (fds[i].revents & POLLIN) {
                taille = read(sockfdlist[i], commande, MAX_TAILLE_BUFFER);
                if (taille == 0) {
                    sclose(sockfdlist[i]);
                    nbSocket--;
                } else {
                    swrite(STDOUT_FILENO, commande, taille);
                    swrite(STDOUT_FILENO,espace,strlen(espace));
                }
            }
        }

        //printf("%d",nbSocket);
        if (nbSocket == 0) {
            kill(getppid(), SIGTERM);
            break;
        }
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("Un argument minimum est nécessaire");
        exit(EXIT_FAILURE);
    }

    pid_t* pids_chils = malloc((argc-1) * sizeof(pid_t));
    int tailleSockFdList = NB_PORTS * (argc - 1);
    int* sockfdlist = smalloc(tailleSockFdList * sizeof(int));
    
    int nbSocket = 0;
    for (int i = 1; i < argc; i++) {
        nbSocket += initSockController(argv[i], sockfdlist, nbSocket);
    }

    for (int i = 0; i < argc-1; i++)
    {
        pids_chils[i] = fork_and_run2(controllerFils, sockfdlist, &nbSocket);
    }

    char commande[MAX_TAILLE_BUFFER];
    int taille, tailleWrite;
    printf("\nEntrez une commande à exécuter : \n");
    while((taille = sread(STDIN_FILENO,commande, MAX_TAILLE_BUFFER))!=0){
        for (int i = 0; i < nbSocket; i++) {
            if (sockfdlist[i] != 0) {
                tailleWrite = write(sockfdlist[i], commande, taille);
                if (tailleWrite == -1) {
                    sockfdlist[i] = 0;
                }
            }
        }
    }

    for(int i = 0; i < argc-1; i++) {
        skill(pids_chils[i],SIGTERM);
    }
    printf("\nSIGTERM\n");

    for (int i = 0; i < nbSocket; i++) {
        if(sockfdlist[i] != 0){
            sclose(sockfdlist[i]);
            printf("close\n");
        }
    }

    free(sockfdlist);
    free(pids_chils);
    printf("free\n");
    return 0;
}
