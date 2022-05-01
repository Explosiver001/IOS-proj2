#	Makefile 
#	Autor: Michal Novak  (xnovak3g@stud.fit.vutbr.cz)

CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -Werror -pedantic -pthread
H_FILES = args.h
ARGS = 3 5 100 100

default: proj2

proj2: proj2.c $(H_FILES) args.o error.o shared_mem.o
	$(CC) $(CFLAGS) proj2.c args.o error.o shared_mem.o -o proj2

args.o: args.c args.h
	$(CC) $(CFLAGS) -c args.c

error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c

shared_mem.o: shared_mem.c shared_mem.h
	$(CC) $(CFLAGS) -c shared_mem.c

run: proj2
	./proj2 $(ARGS)
	@ sleep 1
	@ cat proj2.out

clean: 
	@rm *.o

zip: *.c *.h Makefile
	zip proj2.zip *.c *.h Makefile
