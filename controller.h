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
PRE: sl pointe vers une structure Socket_list valide et initialisée, contenant des descripteurs de socket ouverts.
     nbSockets est un pointeur vers un entier représentant le nombre de sockets dans le tableau sockfdlist
POST: Le processus fils écoute en permanence les sockets pour lesquels il a été créé, affiche les commandes reçues. 
      Et se termine si tous les sockets ont été fermés.

*/
void controllerFils(void* sl, void* nbSockets);

#endif