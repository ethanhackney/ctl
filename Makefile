CFLAGS = -Wall -Werror -pedantic
FFLAGS = $(CFLAGS) -O3
DFLAGS = $(CFLAGS) -DDBUG -fsanitize=address,undefined
SRC    = main.c lib.c
CC     = gcc

safe:
	$(CC) $(DFLAGS) $(SRC)

fast:
	$(CC) $(FFLAGS) $(SRC)
