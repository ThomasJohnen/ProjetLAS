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
#include "reseau.h"
#include "utils_v2.h"

void envoyerCommande(char* commande, int tailleCommande, Socket_list sl) {
    for (int i = 0; i < sl.nbr_sockets; i++) {
        swrite(sl.sockets[i], commande, sizeof(char)*tailleCommande);
    }
}

char** lireReponseCommande(Socket_list sl) {
    char** reps = malloc(sl.nbr_sockets * sizeof(char*));
    for (int i = 0; i < sl.nbr_sockets; i++) {
        char* rep = malloc(100 * sizeof(char));
        sread(sl.sockets[i], rep, 100);
        reps[i] = rep;
    }
    return reps;
}


void controllerFils(Socket_list* sl){
    Socket_list* sockfdlist = sl;
    while(!end){
        char** reps = lireReponseCommande(*sockfdlist);
        for (int i = 0; i < sockfdlist->nbr_sockets; i++) {
            swrite(0,reps[i],strlen(reps));;
        }
    }
}


int main(int argc, char *argv[]) {

    if (argv[1] == NULL) {
        perror("Un argument minimum est nécessaire");
        exit(EXIT_FAILURE);
    }

    char* adresseIp = argv[1];

    Socket_list sockfdlist = initSockController(adresseIp);

    int pid_child = fork_and_run1(controllerFils,&sockfdlist);

    while(!end){
        printf("Entrez une commande à exécuter : \n");
        char* commande = readLine();
        int tailleCommande = strlen(commande);
        envoyerCommande(commande, tailleCommande, sockfdlist);
    }
    


    /*while ((strcmp(buffer,"q")!=0)){
        int tailleCommande = strlen(buffer);
        envoyerCommande(buffer, adresseIp, tailleCommande, sockfdlist);

        // Récupérer les réponses des connexions
        char** rep = lireReponseCommande(sockfdlist);
        // Afficher
        for (int i = 0; i < sockfdlist.nbr_sockets; i++) {
            printf("Réponse %d : %s\n", i, rep[i]);
        }
        // Libérer la mémoire 
        for (int i = 0; i < sockfdlist.nbr_sockets; i++) {
            free(rep[i]);
        }
        free(rep);
        
        printf("Entrez une commande à exécuter: \n");
        buffer = readLine();
    }*/
    /*
    //swrite(sockfd, &adr, sizeof(char)); pas compris a quoi ca servait

    char* commande;//manque des malloc pour ces deux variables je pense
    char* response;
    while (!end ) {
        // lis la ligne de commande
        commande = readLine();
        if(end){
            // envoie le signal a chaque zombie
            for (int i = 0; i < Nb_PORTS; i++)
            {
                if(ports[i] != 0){
                    swrite(sockfd, "stop", sizeof(char));
                }
            }
        }else{
            // envoie la commande a un zombie
            swrite(sockfd, &commande, sizeof(char));
            // envoie la commande a chaque zombies
            //for (int i = 0; i < Nb_PORTS; i++
            //{
            //    if(ports[i] != 0){
            //        swrite(sockfd, &commande, sizeof(char));
            //    }
            //    
            //}
            // lis la reponse de chaque zombie
            sread(sockfd, &response, sizeof(char));
            // affiche la reponse
            swrite(1, &response, sizeof(char));
        }
        
    }
    sclose(sockfd);*/
    for (int i = 0; i < sockfdlist.nbr_sockets; i++) {
        sclose(sockfdlist.sockets[i]);
    }
    printf("%d porttt", PORTS[0]);
    printf("%d porttt", NUM_PORTS);
    return 0;
}