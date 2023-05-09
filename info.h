#ifndef _INFO_H_
#define _INFO_H_

#define KEYSEM 271
#define KEYSHM 322


static int PORTS[] = {5000, 5001, 5002, 5003, 5004, 5005, 5006, 5007, 5008, 5009};
static int NUM_PORTS = 10;

/*const int end = 0;*/

typedef struct Socket_list {
    int* sockets;
    int nbr_sockets;
} Socket_list;

/*typedef struct Zombie_list  {
    int[] sockets;
} Zombie_list;*/

#endif