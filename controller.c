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

volatile sig_atomic_t end = 0;
volatile sig_atomic_t nbHosts = 0;
char* ports[10];

int initSockController(char* adr){
    int sockfd = ssocket();
    int j = 0;
    for (int i = 0; i < NUM_PORTS; i++) {
        printf("%d\n", PORTS[i]);
        // if le port est occupé
        if(sconnect(adr, PORTS[i], sockfd) == 0){
            nbHosts ++;
            ports[j] = PORTS[i];
            j++;
        }
    }

    return sockfd;
    exit(1);
}

void envoyerCommande(char* commande[], int taille, int sockfd) {
    //swrite(sockfd, &taille, sizeof(int));
    swrite(sockfd, commande, sizeof(char)*taille);
    
}

void EndControllerhandler(int num){
    // envoie le signal a chaque zombie
    for (int i = 0; i < ports; i++)
    {
        if(ports[i] != 0){
            
        }
        
    }
    

    end = 1;
}


int main(int argc, char **argv) {

    int port;

    char* text = "test";

    write(0,text,sizeof(text));

    ssigaction(SIGINT, EndControllerhandler);

    if (argv[1] == NULL) {
        perror("Un argument minimum est nécessaire");
        exit(EXIT_FAILURE);
    }

    char* adr = argv[1];

    int sockfd = initSockController(adr);

    char *buffer;
    buffer = readLine();
    while ((strcmp(buffer, "q") != 0)) { // q pour fermer boucle
        envoyerCommande(buffer, strlen(buffer), adr);
        buffer = readLine();

        //...
        // sread(sockfd,...,...);
    }

    // skill(pid_zombie, SIGKILL);
    return 0;
}