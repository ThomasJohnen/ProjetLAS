#ifndef _ZOMBIE_H_
#define _ZOMBIE_H_

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


/**

PRE: commande : une chaîne de caractères représentant une commande à envoyer.
    tailleCommande : la taille de la commande à envoyer en octets.
    sl : une structure Socket_list contenant les descripteurs de sockets destinataires.
POST: Envoie la commande donnée à tous les descripteurs de sockets contenus dans la Socket_list donnée.
*/
void envoyerCommande(char* commande, int tailleCommande, Socket_list sl);

/**
PRE: sl : une structure Socket_list contenant les descripteurs de sockets sources.
POST: La réponse est stockée dans un tableau de pointeurs de chaînes de caractères alloué dynamiquement.
*/
char** lireReponseCommande(Socket_list sl);


/**
PRE: sl pointe vers une structure Socket_list valide et initialisée, contenant des descripteurs de socket ouverts.
POST: Si le socket est fermé, le nombre total de sockets ouverts est mis à jour et la boucle continue jusqu'à ce que tous les sockets soient fermés.
    Si le processus père est toujours en vie, le processus fils est tué et la fonction se termine avec EXIT_SUCCESS.
*/
void controllerFils(void* sl);

#endif