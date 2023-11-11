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

void ParentProcess();
void LikesServer(int num, FILE* file);
char* writeTime();

int main(int argc, char **argv)
{
	ParentProcess();
	return 0;
}



void ParentProcess() {

	FILE* parentFile = fopen("/tmp/ParentProcess", "w");
	FILE* files[10];
	files[0] = fopen("/tmp/LikesServer0", "w");
	files[1] = fopen("/tmp/LikesServer1", "w");
	files[2] = fopen("/tmp/LikesServer2", "w");
	files[3] = fopen("/tmp/LikesServer3", "w");
	files[4] = fopen("/tmp/LikesServer4", "w");
	files[5] = fopen("/tmp/LikesServer5", "w");
	files[6] = fopen("/tmp/LikesServer6", "w");
	files[7] = fopen("/tmp/LikesServer7", "w");
	files[8] = fopen("/tmp/LikesServer8", "w");
	files[9] = fopen("/tmp/LikesServer9", "w");
		
	pid_t LikeServer[10];
	int check = 0;
	char* times[10];
	for (int i = 0; i < 10; i++){
		for (int n = 0; n < i; n++) {
			if (LikeServer[n] == 0) {
				check = 1;
			}
		}
		if (check != 1) {
			LikeServer[i] = fork();
			
			times[i] = writeTime();
			sleep(1);
		}
	}
	for (int m = 0; m < 10; m++) {
		if (LikeServer[m] == 0) {
			printf("LikesServer%d created\n", m);
			fprintf(parentFile, "LikesServer%d started at %s\n", m, times[m]);
			LikesServer(m, files[m]);
			exit(0);
		}
	}
	fprintf(parentFile, "\n");
	int serv[10];
	for (int p = 0; p < 10; p++){
		waitpid(LikeServer[p], &serv[p], 0);
		if (serv[p] != 0) {
			fprintf(parentFile, "LikesServer%d terminated due to failure at %s\n", p, writeTime());
			printf("LikesServer%d terminated due to failure at %s\n", p, writeTime());
		} else {
			fprintf(parentFile, "LikesServer%d successfully terminated at %s\n", p, writeTime());
			printf("LikesServer%d successfully terminated at %s\n", p, writeTime());
		}
	}
	
	printf("All servers terminated\n");
	
	for (int a = 0; a<10; a++) {
		fclose(files[a]);
	}
	fclose(parentFile);
	for (int c = 0; c < 10; c++) {
		free(times[c]);
	}
	
}

void LikesServer(int num, FILE* file) {
	int likes_socket;
	int port = 2000;
	struct sockaddr_in likes_address;
	likes_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (likes_socket == -1) {
		perror("Socket creation failed");
		exit(1);
	}
	likes_address.sin_family = AF_INET;
	likes_address.sin_addr.s_addr =  INADDR_ANY;
	likes_address.sin_port = htons(port);
	
	if (connect(likes_socket, (struct sockaddr*)&likes_address, sizeof(likes_address)) == -1) {
		perror("Connection failed");
		exit(1);
	}
	
	int random;
	int interval;
	srand(time(NULL));
	time_t start, curr;
	double passed = 0.0;
	double limit = 300.0;
	time(&start);
	
	char Server[] = "LikesServer";
	char servNum[3];
	sprintf(servNum, "%d", num);
	char LikesServer[14];
	strcpy(LikesServer, Server);
	strcat(LikesServer, servNum);
	
	char randLike[3];
	int signal = 0;// 0 means you can refresh, 1 means keep adding
	int numLikes = 0;
	while(passed < limit) {   
		if (signal == 0) {
			numLikes = 0;
		} else {
			printf("Signal not received\n");
		}        
		char message[20];
		strcpy(message, LikesServer);
		strcat(message, "    ");
		random = rand() % 43;
		fprintf(file, "Like Count: %d\n", random);
		numLikes = numLikes + random;
		sprintf(randLike, "%d", numLikes);
		strcat(message, randLike);
		interval = 1 + (rand() % 5);
		sleep(interval);
		send(likes_socket, message, strlen(message), 0);
		char response[10];
		int responseSize = recv(likes_socket, response, sizeof(response), 0);
		if (responseSize > 0) {
			signal = 0;
			fprintf(file, "Message received by Primary Server\n");
		} else {
			signal = 1;
		}
		time(&curr);
		passed = difftime(curr, start);
	}	
}



char* writeTime() {
	time_t current_time;
	struct tm* time_info;
	char* time_string = (char*)malloc(80);
	time(&current_time);
	time_info = localtime(&current_time);
	strftime(time_string, 80, "%Y-%m-%d %H:%M:%S", time_info);
	return time_string;
}
