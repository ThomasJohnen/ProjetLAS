// CLIENT
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
#include "utils_v2.h"

int initSockController(int port, char adr){
    // socket creation
    int sockfd = ssocket();
    // prepare socket to connect
    sconnect(adr, port, sockfd);

    return sockfd;
    exit(1);
}

void effectuerCommande(char* commande[], int taille, int sockfd) {
    

    //swrite(sockfd, &taille, sizeof(int));
    swrite(sockfd, commande, sizeof(char)*taille);

    
}


int main(int argc, char **argv) {

    int port; 

    if (argv[1] == NULL) {
        perror("Un argument minimum est nécessaire");
        exit(EXIT_FAILURE);
    }

    char* adr = argv[1];

    for (int i = 0; i < NUM_PORTS; i++) {
        printf("%d\n", PORTS[i]);
        //trouver les ports surlesquels il y a une connexion possible
    }

    initSockController(port, adr);

    char *buffer;
    buffer = readLine();
    while ((strcmp(buffer, "q") != 0)) { // q pour fermer boucle
        effectuerCommande(buffer, strlen(buffer), adr);
        buffer = readLine();
    }

    // skill(pid_zombie, SIGKILL);
    return 0;
}