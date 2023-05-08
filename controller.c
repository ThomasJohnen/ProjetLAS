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

void envoyerCommande(char* commande, char* adr, Socket_list sl) {


}

int main(int argc, char *argv[]) {

    ssigaction(SIGINT, EndControllerhandler);

    if (argv[1] == NULL) {
        perror("Un argument minimum est nécessaire");
        exit(EXIT_FAILURE);
    }

    char* adr = argv[1];

    int sockfd = initSockController(adr);

    swrite(sockfd, &adr, sizeof(char));

    char* commande;
    char* response;
    while (!end ) {
        // lis la ligne de commande
        commande = readLine();
        if(end){
            // envoie le signal a chaque zombie
            for (int i = 0; i < Nb_PORTS; i++)
            {
                if(ports[i] != 0){
                    swrite(sockfd, "stop", sizeof(char));
                }
            }
        }else{
            // envoie la commande a un zombie
            swrite(sockfd, &commande, sizeof(char));
            // envoie la commande a chaque zombies
            /*for (int i = 0; i < Nb_PORTS; i++
            {
                if(ports[i] != 0){
                    swrite(sockfd, &commande, sizeof(char));
                }
                
            }*/
            // lis la reponse de chaque zombie
            sread(sockfd, &response, sizeof(char));
            // affiche la reponse
            swrite(1, &response, sizeof(char));
        }
        
    }
    sclose(sockfd);
    // skill(pid_zombie, SIGKILL);
    return 0;
}