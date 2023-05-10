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

volatile sig_atomic_t end = 0;

void endHandler(int sig)
{
  end = 1;
}

void envoyerCommande(char* commande, int tailleCommande, Socket_list sl) {
    for (int i = 0; i < sl.nbr_sockets; i++) {
        swrite(sl.sockets[i], commande, tailleCommande);
    }
}

char** lireReponseCommande(Socket_list sl) {
    char** reps = malloc(sl.nbr_sockets * sizeof(char*));
    for (int i = 0; i < sl.nbr_sockets; i++) {
        char* rep = malloc(100 * sizeof(char));
        sread(sl.sockets[i], rep, 100);
        reps[i] = rep;
    }
    return reps;
}


void controllerFils(void* sl){
    Socket_list* sockfdlist = sl;
    while(!end){
        char** reps = lireReponseCommande(*sockfdlist);
        for (int i = 0; i < sockfdlist->nbr_sockets; i++) {
            swrite(1,reps[i],strlen(reps[i]))   ;
        }
    }
}


int main(int argc, char *argv[]) {
    ssigaction(SIGINT, endHandler);

    if (argc < 2) {
        perror("Un argument minimum est nécessaire");
        exit(EXIT_FAILURE);
    }

    char* adresseIp = argv[1];

    Socket_list sockfdlist = initSockController(adresseIp);

    fork_and_run1(controllerFils, &sockfdlist);

    char commande[1024];
    while(!end){
        printf("Entrez une commande à exécuter : \n");
        int taille = sread(0,commande, 1024);
        envoyerCommande(commande, taille, sockfdlist);
    }

    for (int i = 0; i < sockfdlist.nbr_sockets; i++) {
        sclose(sockfdlist.sockets[i]);
    }
    printf("%d porttt", PORTS[0]);
    printf("%d porttt", NUM_PORTS);
    return 0;
}