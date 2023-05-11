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

void envoyerCommande(char* commande, int tailleCommande, Socket_list sl) {
    for (int i = 0; i < sl.nbr_sockets; i++) {
        swrite(sl.sockets[i], commande, tailleCommande);
    }
}

char** lireReponseCommande(Socket_list sl) {
    char** reps = malloc(sl.nbr_sockets * sizeof(char*));
    for (int i = 0; i < sl.nbr_sockets; i++) {
        char* rep = malloc(100 * sizeof(char));
        int taille = sread(sl.sockets[i], rep, 100);
        if(taille == 0){
            return NULL;
        }
        reps[i] = rep;
    }
    return reps;
}

void controllerFils(void* sl){
    Socket_list* sockfdlist = sl;
    int nbSocket = sockfdlist->nbr_sockets;
    struct pollfd fds[NB_SOCKET];
    char** reps;
    
    // init poll
	for (int i = 0; i < nbSocket; i++)
	{
		fds[i].fd = sockfdlist->sockets[i];
		fds[i].events = POLLIN;
	}

    while(nbSocket>0){
        int ret = spoll(fds,nbSocket,1000);
        if(ret == 0) continue;
        for (size_t i = 0; i < nbSocket; i++)
        {
            if(fds[i].revents && POLLIN){
                reps = lireReponseCommande(*sockfdlist);
            }
        }
        if (reps == NULL){
            nbSocket--;
        }else{
            for (int i = 0; i < sockfdlist->nbr_sockets; i++) {
                swrite(1,reps[i],strlen(reps[i]))   ;
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

    char* adresseIp = argv[1];

    Socket_list sockfdlist = initSockController(adresseIp);

    int pid_child = fork_and_run1(controllerFils, &sockfdlist);

    char commande[1024];
    int taille;
    char* message = "\nEntrez une commande à exécuter : \n";
    swrite(0,message, strlen(message));
    while((taille = sread(0,commande, 1024))!=0){
        envoyerCommande(commande, taille, sockfdlist);
    }
    skill(pid_child,SIGTERM);
    for (int i = 0; i < sockfdlist.nbr_sockets; i++) {
        sclose(sockfdlist.sockets[i]);
    }

    printf("port %d\n", PORTS[0]);
    printf("num port %d\n", NUM_PORTS);
    return 0;
}