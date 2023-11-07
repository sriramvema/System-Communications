CC = gcc
CFLAGS = -Wall -g

all: PrimaryLikesServer parentProcess

PrimaryLikesServer: PrimaryLikesServer.c
	$(CC) $(CFLAGS) -o PrimaryLikesServer PrimaryLikesServer.c

parentProcess: parentProcess.c
	$(CC) $(CFLAGS) -o parentProcess parentProcess.c
	
clean:
	rm -f PrimaryLikesServer parentProcess