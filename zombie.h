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
PRE: sig : le signal reçu
POST: Met fin au processus zombie du laboratoire en initialisant la variable globale "end" à 1
*/
void endZombieLabo(int sig);

#endif