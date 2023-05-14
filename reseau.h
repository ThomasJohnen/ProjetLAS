#ifndef _RESEAU_H_
#define _RESEAU_H_

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

/**

PRE: adr : C'est une chaîne de caractères représentant une adresse IP valide
     sockFdList : C' est un tableau d'entiers initialisé de taille au moins NB_PORTS
     start_index : C'est un entier représentant l'indice de départ pour stocker les descripteurs de sockets dans le tableau sockFdList
POST: Retourne le nombre de sockets créés et connectés avec succès aux adresses IP et ports spécifiés
      Les descripteurs de sockets créés et connectés avec succès sont stockés dans sockFdList à partir de l'indice start_index
      Toutes les autres entrées de sockFdList à partir de l'indice start_index + le nombre de sockets connectés avec succès sont mises à zéro
*/

int initSockController(char* adr, int* sockFdList, int start_index);

/**
PRE: Aucune précondition n'est requise.
POST: Retourne le descripteur de fichier du socket initialisé.

*/
int initSocketZombie();

#endif 