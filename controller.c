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

/*void envoyerCommande(char* commande, int tailleCommande, int* sl) {
    for (int i = 0; i < sizeof(*sl); i++) {
           swrite(sl[i], commande, tailleCommande); 
    }
}*/

void controllerFils(void* sl){
    int* sockfdlist = sl;
    int realNbSockets = 0;
    for (int i = 0; i < sizeof(sockfdlist); i++)
    {
        if(sockfdlist[i] != 0) realNbSockets ++;
    }

    struct pollfd fds[NB_SOCKET];
    char commande[MAX_TAILLE_BUFFER];
    int nbSocket = sizeof(sockfdlist);
    int taille;
    
	for (int i = 0; i < nbSocket; i++)
	{
		fds[i].fd = sockfdlist[i];
		fds[i].events = POLLIN;
	}

    printf("%d", realNbSockets);
    while(realNbSockets>0){
        int ret = spoll(fds,nbSocket,1000);
        if(ret == 0) continue;

        for (int i = 0; i < nbSocket; i++)
        {
            if(fds[i].revents && POLLIN){
                taille = sread(sockfdlist[i], commande, MAX_TAILLE_BUFFER);
                if(taille == 0){
                    realNbSockets --;
                    printf("%d", realNbSockets);
                }else{
                    nwrite(1, commande, taille);
                    char* espace = "\n";
                    nwrite(STDOUT_FILENO,espace,strlen(espace));
                }
            }
        }
    }
    printf("%d", realNbSockets);
    skill(getppid(),SIGTERM);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("Un argument minimum est nécessaire");
        exit(EXIT_FAILURE);
    }

    int tailleSockFdList = (argc-1)*NB_PORTS;
    
    int* sockfdlist = smalloc(tailleSockFdList * sizeof(int));

    sockfdlist = initSockController(argv[1]);

    pid_t pids_chils = fork_and_run1(controllerFils, sockfdlist);
    
    char commande[MAX_TAILLE_BUFFER];
    int taille;
    printf("\nEntrez une commande à exécuter : \n");
    while((taille = sread(0,commande, MAX_TAILLE_BUFFER))!=0){
        for (int i = 0; i < tailleSockFdList; i++)
        {
            if(sockfdlist[i] != 0){
                nwrite(sockfdlist[i], commande, taille);
                /*char* test = "\necriture\n";
                nwrite(STDOUT_FILENO,test,strlen(test));*/
            } 
        }
    }

    skill(pids_chils,SIGTERM);
    printf("\nSIGTERM\n");

    for (int i = 0; i < tailleSockFdList; i++) {
        if(sockfdlist[i] != 0){
            
            sclose(sockfdlist[i]);
            printf("close\n");
        }
    }

    free(sockfdlist);
    printf("free\n");
    return 0;
}