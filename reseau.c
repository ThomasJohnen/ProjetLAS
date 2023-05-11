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



/*void endServerHandler(int sig)
{
  end = 1;
}*/

Socket_list initSockController(char* adr)
{ //connexion aux serveur d'écoute
    int* ports = smalloc(NUM_PORTS * sizeof(int));
    int nbHosts = 0;

    for(int i = 0; i < NUM_PORTS; i++){
        int sockfd = ssocket();

        // s'adapter au connect et pas sconnect
        struct sockaddr_in addr;
        memset(&addr,0,sizeof(addr)); /* en System V */
        addr.sin_family = AF_INET;
        addr.sin_port = htons(PORTS[i]);
        inet_aton(adr,&addr.sin_addr);

        if(connect(sockfd, (struct sockaddr *) &addr, sizeof(addr))==0){
            ports[nbHosts] = sockfd;
            nbHosts ++;
            printf("port %d : %d\n", nbHosts, PORTS[i]);
        }else {
            sclose(sockfd);
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


