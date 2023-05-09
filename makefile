CC=gcc

CCFLAGS=-D_DEFAULT_SOURCE -D_XOPEN_SOURCE -D_BSD_SOURCE -std=c11 -pedantic -Wvla -Wall -Werror

ALL= zombie controller

all: $(ALL)

zombie : zombie.o utils_v2.o reseau.o
	$(CC) $(CCFLAGS) -o zombie zombie.o utils_v2.o reseau.o

zombie.o : zombie.c info.h
	$(CC) $(CCFLAGS) -c zombie.c

controller : controller.o utils_v2.o reseau.o
	$(CC) $(CCFLAGS) -o controller controller.o utils_v2.o	reseau.o

controller.o : controller.c info.h
	$(CC) $(CCFLAGS) -c controller.c

reseau.o : reseau.c info.h
	$(CC) $(CCFLAGS) -c reseau.c

utils_v2.o: utils_v2.c utils_v2.h
	$(CC) $(CCFLAGS) -c utils_v2.c	

clean:
	rm -f *.o
	rm -f $(ALL)