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
#define MAX_TAILLE_BUFFER 1024

void envoyerCommande(char* commande, int tailleCommande, int* sl) {
    for (int i = 0; i < NB_ADRESSES*10; i++) {
        swrite(sl[i], commande, tailleCommande);
    }
}

void controllerFils(void* sl){
    int* sockfdlist = sl;
    int nbSocket = sizeof(*sockfdlist);
    struct pollfd fds[NB_SOCKET];
    char commande[MAX_TAILLE_BUFFER];
    
	for (int i = 0; i < nbSocket; i++)
	{
		fds[i].fd = sockfdlist[i];
		fds[i].events = POLLIN;
	}
    while(nbSocket>0){
        int ret = spoll(fds,nbSocket,1000);
        if(ret == 0) continue;

        for (int i = 0; i < nbSocket; i++)
        {
            if(fds[i].revents && POLLIN){
                int taille = sread(sockfdlist[i],commande,strlen(commande));
                if(taille == 0){
                    nbSocket --;
                }
                swrite(1,commande,strlen(commande));
            }
        }
    }
    skill(getppid(),SIGTERM);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("Un argument minimum est nécessaire");
        exit(EXIT_FAILURE);
    }

    char** adressesIp = malloc((argc) * sizeof(char*));
    char* pids_chils = malloc((argc) * sizeof(char));
    int* sockfdlist = malloc((argc*10) * sizeof(int));

    for (int i = 0; i < argc-1; i++)
    {
        adressesIp[i] = argv[i+1];
        sockfdlist = initSockController(&adressesIp[i]);
        pids_chils[i] = fork_and_run1(controllerFils, &sockfdlist);
    }
    
    char commande[MAX_TAILLE_BUFFER];
    int taille;
    printf("\nEntrez une commande à exécuter : \n");
    while((taille = sread(0,commande, MAX_TAILLE_BUFFER))!=0){
        envoyerCommande(commande, taille, sockfdlist);
    }
    for (int i = 0; i < argc-1; i++)
    {
        skill(pids_chils[i],SIGTERM);
    }
    for (int i = 0; i < argc-1; i++) {   
        sclose(sockfdlist[i]);
    }

    free(adressesIp);
    free(pids_chils);
    free(sockfdlist);
    return 0;
}