#ifndef _LABO_H_
#define _LABO_H_

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdarg.h>
#include <time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <signal.h>
#include <poll.h>

#include "utils_v2.h"
#include "zombie.h"
#include "info.h"

/**
* Processus fils qui va lancer un la programme zombie
* PRE: p : l'emplacement mémoire de la chaine de charactère qui représente
*           le port via lequel le zombie va être lancé.
* POST: le promme zombie est lancé et tourne sur le port spécifié
*/
void childLabo(void *p);


#endif