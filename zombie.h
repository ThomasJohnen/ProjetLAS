#ifndef _ZOMBIE_H_
#define _ZOMBIE_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

#include "utils_v2.h"
#include "info.h"
#include "reseau.h"

/**
* processus fils qui va s'occuper de lancer un terminal et renvoyer les reponses via le socket en parametre
* PRE: sockfdController : un pointeur void qui pointe vers un entier représentant le descripteur de fichier de socket.
* POST: Les entrées/sorties standard (STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO) sont redirigées vers le descripteur de fichier de socket fourni en entrée.
*/
void zombieFils(void* sockfdController);

/**
* fermeture du programme zombie et des ses processus fils
*/
void endZombieLabo(int sig);

#endif