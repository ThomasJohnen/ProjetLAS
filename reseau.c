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
#include "reseau.h"


int initSockController(char* adr, int* sockFdList, int start_index){
    int nbHosts = 0;

    for(int i = 0; i < NB_PORTS; i++){
        int sockfd = ssocket();

        struct sockaddr_in addr;
        memset(&addr,0,sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(PORTS[i]);
        inet_aton(adr,&addr.sin_addr);

        if(connect(sockfd, (struct sockaddr *) &addr, sizeof(addr))==0){
            sockFdList[start_index + nbHosts] = sockfd;
            nbHosts ++;
            printf("port %d : %d\n", nbHosts, PORTS[i]);
        }
    }
    printf("\nnombre de hosts : %d\n", nbHosts);
    return nbHosts;
}

int initSocketZombie()
{ // intialise serveur d'écoute
  int sockfd = ssocket();
  int initiated = 0;
  while (initiated==0){
    
    //choisir port aléatoirement
    srand(time(NULL));
    int randomIndex = rand() % NB_PORTS;
    int selectedPort = PORTS[randomIndex];

    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(selectedPort);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //tester si port déjà pris
    if (bind(sockfd, (struct sockaddr *) &addr, sizeof(addr))!= -1){
        initiated = 1;
        printf("Port sélectionné : %d\n", selectedPort);
    }
  }
  slisten(sockfd,5);
  return sockfd;
}


