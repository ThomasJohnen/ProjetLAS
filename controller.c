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

//#define Nb_PORTS 10

volatile sig_atomic_t end = 0;
//int* ports[Nb_PORTS];

volatile int nbHosts = 0;

int* ports = smalloc(sizeof(int)*NUM_PORTS*10);

void initSockController(char* adr){
    for(int i = 0; i < NUM_PORTS; i++){
        if(sconnect(adr, PORTS[i], sockfd)==0){
            int sockfd = ssocket();
            ports[nbHosts] = sockfd;
            printf("port %d : %d\n", nbHosts, PORTS[i])
        nbHosts ++;
        }
    }
    
    printf("\nnombre de hosts : %d\n", nbHosts);
}

void EndControllerhandler(int num){
    end = 1;
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