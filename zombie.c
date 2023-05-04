#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

#include "utils_v2.h"
#include "info.h"

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

void programme_inoffensif(void* adrr, void* portt, void *pipefdd) {
    // pas sur des arguments de la methode
    char* adr = adrr;
    int* port = portt;
    int* pipefd = pipefdd;
    //...
    FILE *fp = popen("/bin/bash", "r"); // Ouvrir un terminal bash
    if (fp == NULL) {
        perror("popen");
        return 1;
    }

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
    
    
    int port = choisirPort();
    int sockfd = initSocketZombie(port);

    int shm_id = sshmget(KEYSHM,1000*sizeof(int),0);
    int sem_id = sem_get(KEYSEM,0);
    int *z = sshmat(shm_id);

    int pipefd[2];
    int spipe(pipefd);
    char* adr;
    int pid_zombie = fork_and_run3(programme_inoffensif, &adr, &port, pipefd);

    
    



    return 0;
    
}