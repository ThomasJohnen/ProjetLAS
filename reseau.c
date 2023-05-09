#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>


#include "info.h"
#include "utils_v2.h"



/*void endServerHandler(int sig)
{
  end = 1;
}*/

Socket_list initSockController(char* adr)
{ //connexion aux serveur d'écoute
    int* ports = malloc(NUM_PORTS * sizeof(int));
    int nbHosts = 0;

    for(int i = 0; i < NUM_PORTS; i++){
        int sockfd = ssocket();
        if(sconnect(adr, PORTS[i], sockfd)==0){
            ports[nbHosts] = sockfd;
            nbHosts ++;
            printf("port %d : %d\n", nbHosts, PORTS[i]);
        } else {
            close(sockfd);
        }
    }
    
    printf("\nnombre de hosts : %d\n", nbHosts);
    Socket_list sl;
    sl.sockets = ports;
    sl.nbr_sockets = nbHosts;
    return sl;
}

int initSocketZombie()
{ // intialise serveur d'écoute

  int sockfd = ssocket();
  int initiated = 0;
  while (initiated==0){
    
    //choisir port aléatoirement
    srand(time(NULL));
    int randomIndex = rand() % NUM_PORTS;
    int selectedPort = PORTS[randomIndex];
    printf("Port sélectionné : %d\n", selectedPort);

    //tester si port déjà pris
    if (sbind(selectedPort,sockfd)!= -1){
        initiated = 1;
    }

  }
  slisten(sockfd,5);
  return sockfd;
}


