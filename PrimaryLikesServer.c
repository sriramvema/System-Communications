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


int main(int argc, char **argv)
{
	int primary_socket, likes_socket;
	struct sockaddr_in primary_address, likes_address;
	socklen_t likes_address_len = sizeof(likes_address);
	
	primary_socket = socket(AF_INET, SOCK_STREAM, 0);
	
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
	
	if (listen(primary_socket, 1) == -1) {
		perror("Socket listening failed");
		exit(1);
	}
	
	likes_socket = accept(primary_socket, (struct sockaddr*)&likes_address, &likes_address_len);
	if (likes_socket == -1) {
		perror("Socket accept failed");
		exit(1);
	}
	
	FILE* primaryFile = fopen("/tmp/PrimaryLikesServer", "w");
	char received[50];
	char copied[50];
	ssize_t received_size;
	int total = 0;
	fprintf(primaryFile, "Starting total: %d\n", total);
	int addLike = 0;
	srand(time(NULL));
	
	while(1) {
		received_size = recv(likes_socket, received, sizeof(received), 0);
		if (received_size <= 0) {
			printf("No message received\n");
			break;
		}
		received[received_size] = '\0';
		strcpy(copied, received);
		char *token = strtok(received, " ");
		token = strtok(NULL, " ");
		addLike = atoi(token);
		total = total + addLike;
		fprintf(primaryFile, "%s, Total Likes: %d\n", copied, total);
		char* signal = "received";
		send(likes_socket, signal, strlen(signal), 0);
			

		
		
	}
	printf("All LikesServers terminated.");
	fclose(primaryFile);
	return 0;
}
