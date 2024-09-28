#############################################################
# Makefile
# Projekt: Implementace překladače imperativního jazyka IFJ24
# Tvůrci: Ivo Puchnar, xpuchn02
#############################################################

all: *.o
	gcc -g -std=gnu99 -Wall -Wextra -Werror -o prekladac *.o
	rm -f *.o

*.o: *.c
	gcc -std=gnu99 -Wall -Wextra -Werror -c *.c

clean:
	rm -f prekladac
