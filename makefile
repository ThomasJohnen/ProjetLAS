CC=gcc

CCFLAGS=-D_DEFAULT_SOURCE -D_XOPEN_SOURCE -D_BSD_SOURCE -std=c11 -pedantic -Wvla -Wall -Werror

ALL= zombie controller info

all: $(ALL)

zombie : zombie.o utils_v2.o 
	$(CC) $(CCFLAGS) -o zombie zombie.o utils_v2.o

zombie.o : zombie.c info.h
	$(CC) $(CCFLAGS) -c zombie.c

info : info.o 
	$(CC) $(CCFLAGS) -o info info.o

info.o : info.c info.h
	$(CC) $(CCFLAGS) -c info.c	

controller : controller.o utils_v2.o 
	$(CC) $(CCFLAGS) -o controller controller.o utils_v2.o	

controller.o : controller.c info.h
	$(CC) $(CCFLAGS) -c controller.c	

clean:
	rm -f *.o
	rm -f $(ALL)