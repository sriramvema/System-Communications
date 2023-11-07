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
void LikesServer(int num, FILE* file, int socket);
void writeTime(FILE* file, int exit, int num);

int main(int argc, char **argv)
{
	ParentProcess();
	return 0;
}



void ParentProcess() {
	int likes_socket;
	struct sockaddr_in likes_address;
	likes_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (likes_socket == -1) {
		perror("Socket creation failed");
		exit(1);
	}
	likes_address.sin_family = AF_INET;
	likes_address.sin_addr.s_addr =  INADDR_ANY;
	likes_address.sin_port = htons(2000);
	
	if (connect(likes_socket, (struct sockaddr*)&likes_address, sizeof(likes_address)) == -1) {
		perror("Connection failed");
		exit(1);
	}
		

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
	
	/*LikesServer0 = fork();
	if (LikesServer0 == 0) {
		writeTime(parentFile, 0, 0);
		LikesServer(0, file0, likes_socket);
		writeTime(parentFile, 1, 0);
		exit(0);
		
	} else {
	
		
		sleep(1);
		LikesServer1 = fork();
		
		
		if (LikesServer1 == 0) {
			writeTime(parentFile, 0, 1);
			LikesServer(1, file1, likes_socket);
			writeTime(parentFile, 1, 1);
			exit(0);
			
		} else {
			
			sleep(1);
				
			LikesServer2 = fork();
			
			
			if (LikesServer2 == 0) {
				writeTime(parentFile, 0, 2);
				LikesServer(2, file2, likes_socket);
				writeTime(parentFile, 1, 2);
				exit(0);
			} else {
				
				sleep(1);
				
				LikesServer3 = fork();
				
				
				if (LikesServer3 == 0) {
					writeTime(parentFile, 0, 3);
					LikesServer(3, file3, likes_socket);
					writeTime(parentFile, 1, 3);
					exit(0);
					
				} else {
					
					sleep(1);
					
					LikesServer4 = fork();
					
					
					if (LikesServer4 == 0) {
						writeTime(parentFile, 0, 4);
						LikesServer(4, file4, likes_socket);
						writeTime(parentFile, 1, 4);
						exit(0);
						
					} else {
						
						sleep(1);
						
						LikesServer5 = fork();
						
						
						if (LikesServer5 == 0) {
							writeTime(parentFile, 0, 5);
							LikesServer(5, file5, likes_socket);
							writeTime(parentFile, 1, 5);
							exit(0);
							
						} else {
							
							sleep(1);
							
							LikesServer6 = fork();
							
							if (LikesServer6 == 0) {
								writeTime(parentFile, 0, 6);
								LikesServer(6, file6, likes_socket);
								writeTime(parentFile, 1, 6);
								exit(0);
								
							} else {
								
								sleep(1);
								
								LikesServer7 = fork();
								
								
								if (LikesServer7 == 0) {
									writeTime(parentFile, 0, 7);
									LikesServer(7, file7, likes_socket);
									writeTime(parentFile, 1, 7);
									exit(0);
									
								} else{
									
									sleep(1);
									
									LikesServer8 = fork();
									
									if (LikesServer8 == 0) {
										writeTime(parentFile, 0, 8);
										LikesServer(8, file8, likes_socket);
										writeTime(parentFile, 1, 8);
										exit(0);
									} else {
										
										sleep(1);
										
										LikesServer9 = fork();
						
										if (LikesServer9 == 0) {
											writeTime(parentFile, 0, 9);
											LikesServer(9, file9, likes_socket);
											writeTime(parentFile, 1, 9);
											exit(0);
											
										}	
									}
								}
							}
						}					
					}
				}			
			}		
		}	
	}
	if (LikesServer0 > 0) {
		int serv0, serv1, serv2, serv3, serv4, serv5, serv6, serv7, serv8, serv9;
		waitpid(LikesServer0, &serv0, 0);
		waitpid(LikesServer1, &serv1, 0);
		waitpid(LikesServer2, &serv2, 0);
		waitpid(LikesServer3, &serv3, 0);
		waitpid(LikesServer4, &serv4, 0);
		waitpid(LikesServer5, &serv5, 0);
		waitpid(LikesServer6, &serv6, 0);
		waitpid(LikesServer7, &serv7, 0);
		waitpid(LikesServer8, &serv8, 0);
		waitpid(LikesServer9, &serv9, 0);
		if (serv1 == 0 && serv2 == 0 && serv3 == 0 && serv4 == 0 && serv5 == 0 && serv6 == 0 && serv7 == 0 && serv8 == 0 && serv9 == 0 && serv0 == 0) {
		printf("All processes terminated\n");
		
		fclose(file0);
		fclose(file1);
		fclose(file2);
		fclose(file3);
		fclose(file4);
		fclose(file5);
		fclose(file6);
		fclose(file7);
		fclose(file8);
		fclose(file9);
		fclose(parentFile);
		close(likes_socket);
		}
	}*/
	
	pid_t LikeServer[10];
	int check = 0;
	for (int i = 0; i < 10; i++){
		for (int n = 0; n < i; n++) {
			if (LikeServer[n] == 0) {
				check = 1;
			}
		}
		if (check != 1) {
			LikeServer[i] = fork();
			writeTime(parentFile, 0, i);
			sleep(1);
		}
	}
	for (int m = 0; m < 10; m++) {
		if (LikeServer[m] == 0) {
			LikesServer(m, files[m], likes_socket);
			writeTime(parentFile, 1, m);
			exit(0);
		}
	}
	int serv[10];
	for (int p = 0; p < 10; p++){
		waitpid(LikeServer[p], &serv[p], 0);
	}
	check = 0;
	for (int z = 0; z < 10; z++) {
		if (serv[z] != 0) {
			check = 1;
		}
	}
	if (check == 0) {
		printf("All servers terminated");
	}
	for (int a = 0; a<10; a++) {
		fclose(files[a]);
	}
	fclose(parentFile);
	
	
	
}

void LikesServer(int num, FILE* file, int socket) {
	
	int random;
	int interval;
	srand(time(NULL));
	time_t start, curr;
	double passed = 0.0;
	double limit = 30.0;
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
		}        
		char message[20];
		strcpy(message, LikesServer);
		strcat(message, " ");
		random = rand() % 43;
		fprintf(file, "Like Count: %d\n", random);
		numLikes = numLikes + random;
		sprintf(randLike, "%d", numLikes);
		strcat(message, randLike);
		interval = 1 + (rand() % 5);
		sleep(interval);
		send(socket, message, strlen(message), 0);
		char response[10];
		int responseSize = recv(socket, response, sizeof(response), 0);
		if (responseSize > 0) {
			signal = 0;
		} else {
			signal = 1;
		}
		time(&curr);
		passed = difftime(curr, start);
	}
	printf("LikesServer%d\n", num);
		
}



void writeTime(FILE* file, int exit, int num) {
	time_t current_time;
	struct tm* time_info;
	char* time_string = (char*)malloc(80);
	time(&current_time);
	time_info = localtime(&current_time);
	strftime(time_string, 80, "%Y-%m-%d %H:%M:%S", time_info);
	
	if (exit == 0) {
		fprintf(file, "LikesServer%d started at %s\n", num, time_string);
	} else {
		fprintf(file, "LikesServer%d terminated at %s\n", num, time_string);
	}
	free(time_string);
}
