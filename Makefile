CC=gcc
CFLAGS=-c -O -Wall
LDFLAGS=

SRC=cipher.c sdes.c
OBJ=$(SRC:.c=.o)
HDR=sdes.h
EXEC=cipher



all: $(SRC) $(EXEC)
	
$(EXEC): $(OBJ) 
		$(CC) $(LDFLAGS) $(OBJ) -o $@ -lm

%.o:	%.c $(HDR)
		$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ) $(EXEC) *~ core

