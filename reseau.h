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

PRE: adr : une chaîne de caractères représentant une adresse IP valide.
POST: Retourne une structure Socket_list contenant les descripteurs de socket des ports qui sont connectés avec succès à l'adresse IP.
*/
Socket_list initSockController(char* adr);


/**
PRE: Aucune précondition n'est requise.
POST: Retourne le descripteur de fichier du socket initialisé.

*/
int initSocketZombie();

#endif 