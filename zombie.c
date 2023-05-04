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

void programme_inoffensif(void* adrr, void* portt, void* newsockfdd) {
    int* newsockfd = newsockfdd;
    char* commande;
    char* response;
    printf("Début du programme inoffensif\n");
    /*FILE *fp = popen("/bin/bash", "r"); // Ouvrir un terminal bash
    if (fp == NULL) {
        perror("popen");
        exit(1);
    }*/

    printf("/bin/bash ouvert\n");
    while(end!=1){
        // lis ce que lui envoie le controller
        sread(*newsockfd, &commande, sizeof(char));

        if(strcmp(commande, "stop") == 0){
            end = 1;
            printf("Fin du programme inoffensif\n");
        }else{
            // execute la commande dans le terminal
            printf("Execution de la commande\n");
            /*response = fprintf(fp, "%s", commande);*/
            /*response = */fprintf(stdout, "%s", commande);
            // renvoie le resultat de la commande au controller
            swrite(*newsockfd, &response, sizeof(char));
        }
        sleep(1);
    }
    //pclose(fp); 
}

int main(int argc, char *argv[]){
    int port;

    if(argv[1] == NULL){
        port = choisirPort();
    }else{
        port = atoi(argv[1]);
    }
    
    int sockfd = initSocketZombie(port);

    int newsockfd = saccept(sockfd);
    printf("Connexion établie\n");

    char* adr;
    sread(newsockfd, &adr, sizeof(char));

    printf("Adresse du controller : %s\n", adr);

    int pid_zombie = fork_and_run3(programme_inoffensif, &adr, &port, &newsockfd);

    /* Attente de la terminaison du processus fils */
    int status;
    swaitpid(pid_zombie, &status, 0);

    sclose(newsockfd);

    return 0;
    
}