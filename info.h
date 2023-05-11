#ifndef _INFO_H_
#define _INFO_H_

static const int PORTS[] = {5000, 5001, 5002, 5003, 5004, 5005, 5006, 5007, 5008, 5009};
static const int NUM_PORTS = 10;

typedef struct Socket_list {
    int* sockets;
    int nbr_sockets;
} Socket_list;

#endif