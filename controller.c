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

void effectuerCommande(char* commande[], int taille, char *adr, int port) {
    int sockfd = ssocket();
    sconnect(adr, port, sockfd);

    //swrite(sockfd, &taille, sizeof(int));
    swrite(sockfd, commande, sizeof(char)*taille);

    sclose(sockfd);
}


int main(int argc, char **argv) {


    if (argv[1] == NULL) {
        perror("Un argument minimum est nécessaire");
        exit(EXIT_FAILURE);
    }

    char* adr = argv[1];

    for (int i = 0; i < NUM_PORTS; i++) {
        printf("%d\n", PORTS[i]);
        //trouver les ports surlesquels il y a une connexion possible
    }
    char *buffer;
   
    buffer = readLine();
    sclose(pipefd[0]);
    while ((strcmp(buffer, "q") != 0)) { // q pour fermer boucle
        effectuerCommande(buffer, strlen(buffer), adr, port);
        buffer = readLine();
    }

    skill(pid_zombie, SIGKILL);
    sclose(pipefd[1]);
    return 0;
}