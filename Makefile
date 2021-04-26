CC = gcc
CFLAGS = -Wall -Werror -Wpedantic \
         -Wformat=2 -Wno-unused-parameter -Wshadow \
	 -Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
         -Wredundant-decls -Wnested-externs -Wmissing-include-dirs
SRC = bsh.c
OBJ = bsh

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OBJ)

clean:
	$(RM) $(OBJ)
