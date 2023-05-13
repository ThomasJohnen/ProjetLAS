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

void childLabo(void *p){
    char *port = p;
    sexecl("./zombie","zombie", port, NULL);
}


int main(int argc, char *argv[]){
    int random1, random2;
    do {
        random1 = randomIntBetween(0, 9);
        random2 = randomIntBetween(0, 9);
    } while (random1 == random2);

    char port1[5];
    char port2[5];
    sprintf(port1, "%d", PORTS[random1]);
    sprintf(port2, "%d", PORTS[random2]);
    
    pid_t childLaboId1 = fork_and_run1(childLabo, port1);
    pid_t childLaboId2 = fork_and_run1(childLabo, port2);

    while (true)
    {
        char *s = smalloc(100*sizeof(char));
        int size = sread(STDIN_FILENO, s, strlen(s));

        if (size == 0){
            break;
        }
    }

    skill(childLaboId1, SIGTERM);
    skill(childLaboId2, SIGTERM);

    swait(&childLaboId1);
    swait(&childLaboId2);

    exit(0);
}