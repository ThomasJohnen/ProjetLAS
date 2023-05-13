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

void childLabo(void *arg1)
{
    char *port = (char *)(arg1);
    zombie(port);
}


int main(int argc, char const *argv[])
{
    // creation de 2 nombre aléatoire
    int random1 = randomIntBetween(0, 9);
    int random2 = randomIntBetween(0, 9);

    // il faut vérifier que les 2 nombres soient différent
    if (random1 == random2)
    {
        while (random1 == random2)
        {
            random2 = randomIntBetween(0, 9);
        }
    }

    // On crée les 2 ports avec les 2 nombres aléatoire en char
    char port1[] = "5000";
    port1[4] = (random1 + '0');
    char port2[] = "5000";
    port2[4] = (random2 + '0');

    // lancer les 2 zombies
    pid_t childLaboId1 = fork_and_run1(childLabo, port1);
    pid_t childLaboId2 = fork_and_run1(childLabo, port2);

    // lis s'il y a un Ctrl+D
    while (true)
    {
        char *s = smalloc(BUFFER_SIZE*sizeof(char));
        int size = sread(STDIN_FILENO, s, strlen(s));

        // zise = 0 -> Ctrl+D
        if (size == 0)
        {
            break;
        }
    }

    // tue les deux serveurs (zombies)
    skill(childLaboId1, SIGTERM);
    skill(childLaboId2, SIGTERM);

    // attend que tout soit fini avant de ce finir
    swait(&childLaboId1);
    swait(&childLaboId2);

    exit(0);
}



//add a comment to commit