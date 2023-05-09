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

void endServerHandler(int sig);

Socket_list initSockController(char* adr);

int initSocketZombie();

#endif 