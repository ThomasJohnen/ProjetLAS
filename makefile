CC=gcc

CCFLAGS=-D_DEFAULT_SOURCE -D_XOPEN_SOURCE -D_BSD_SOURCE -std=c11 -pedantic -Wvla -Wall -Werror

ALL= zombie controller labo

all: $(ALL)

labo: labo.o utils_v2.o zombie.o
	gcc $(CFLAGS) -o labo labo.o utils_v2.o zombie.o 

labo.o: labo.c labo.h utils_v2.h  zombie.h
	gcc $(CFLAGS) -c labo.c

zombie : zombie.o utils_v2.o reseau.o
	$(CC) $(CCFLAGS) -o zombie zombie.o utils_v2.o reseau.o

zombie.o : zombie.c info.h zombie.h
	$(CC) $(CCFLAGS) -c zombie.c

controller : controller.o utils_v2.o reseau.o
	$(CC) $(CCFLAGS) -o controller controller.o utils_v2.o	reseau.o

controller.o : controller.c info.h controller.h
	$(CC) $(CCFLAGS) -c controller.c

reseau.o : reseau.c info.h reseau.h
	$(CC) $(CCFLAGS) -c reseau.c

utils_v2.o: utils_v2.c utils_v2.h
	$(CC) $(CCFLAGS) -c utils_v2.c	

clean:
	rm -f *.o
	rm -f $(ALL)