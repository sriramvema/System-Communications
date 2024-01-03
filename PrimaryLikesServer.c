#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	int primary_socket;
	struct sockaddr_in primary_address;

	primary_socket = socket(AF_INET, SOCK_STREAM, 0);
	int likes_sockets[10];
	for (int i = 0; i < 10; i++) {
		likes_sockets[i] = 0;
	}
	if (primary_socket == -1) {
		perror("Socket creation failed");
		exit(1);
	}
	
	primary_address.sin_family = AF_INET;
	primary_address.sin_addr.s_addr = INADDR_ANY;
	primary_address.sin_port = htons(2000);
	
	if (bind(primary_socket, (struct sockaddr*)&primary_address, sizeof(primary_address)) == -1) {
		perror("Socket binding failed");
		exit(1);
	}
	
	if (listen(primary_socket, 10) == -1) {
		perror("Socket listening failed");
		exit(1);
	}
	FILE* primaryFile = fopen("/tmp/PrimaryLikesLog", "w");
	int total = 0;
	fprintf(primaryFile, "Starting total  %d\n\n", total);
	int count = 0;
	int closed = 0;
	while(1) {
		if (closed == 10){
			fprintf(primaryFile, "Ending total    %d\n", total);
			fclose(primaryFile);
		}
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(primary_socket, &readfds);
		int max_sd = primary_socket;
		
		for (int i = 0; i < 10; i++) {
			FD_SET(likes_sockets[i], &readfds);
			if (likes_sockets[i] > max_sd) {
				max_sd = likes_sockets[i];
			}
		}
		
		select(max_sd + 1, &readfds, NULL, NULL, NULL);
		
		if (FD_ISSET(primary_socket, &readfds)) {
			int new_likes = accept(primary_socket, NULL, NULL);
			likes_sockets[count]=new_likes;
			printf("LikesServer%d added!\n", count);
			count++;
		}
		
		for (int n = 0; n < 10; n++) {
			int sd = likes_sockets[n];
			if (sd != 0){
				if (FD_ISSET(sd, &readfds)) {
					char received[1024];
					char copied[1024];
					int size = read(sd, received, 1024);
					if (size == 0) {
						printf("LikesServer%d Closed\n", n);
						close(sd);
						likes_sockets[n] = 0;
						closed++;
					} else {
						received[size] = '\0';
						strcpy(copied, received);
						char *token = strtok(received, " ");
						token = strtok(NULL, " ");
						int addLike = atoi(token);
						total = total + addLike;
						fprintf(primaryFile, "%s\nTotal           %d\n\n", copied, total);
						//fclose(primaryFile);
						char* signal = "1";
						send(sd, signal, strlen(signal), 0);
					}
				}
			}
			
		}
	}
	return 0;
}
