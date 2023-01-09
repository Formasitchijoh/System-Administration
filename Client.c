#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

void * doRecieving(void * sockID){

	int clientSocket = *((int *) sockID);

	while(1){

		char data[1024];
		int read = recv(clientSocket,data,1024,0);
		data[read] = '\0';
		printf("%s\n",data);

	}

}

char* concat_string(char s[], char s1[]){
	int i;
	int j = strlen(s);
	for(i=0; s1[i] != '\0'; i++){
		s[i+j] = s1[i];
	}
	
	s[i+j] = '\0';
	
	return s;
}

int main(){

	int clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serverAddr;

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8080);
	serverAddr.sin_addr.s_addr = inet_addr("192.168.230.33");

	if(connect(clientSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) == -1) return 0;

	printf("Connection established ............\n");

	pthread_t thread;
	pthread_create(&thread, NULL, doRecieving, (void *) &clientSocket );

	while(1){

		char input[1024];
		char input1[1024];
		char input2[1024];
		scanf("%s",input);

		if(strcmp(input,"LIST") == 0){

			send(clientSocket,input,1024,0);

		}
		if(strcmp(input,"SEND") == 0){

			send(clientSocket,input,1024,0);
			
			scanf("%s",input);
			send(clientSocket,input,1024,0);
			
			scanf("%[^\n]s",input);
			send(clientSocket,input,1024,0);

		}
		if(strcmp(input,"CONNECT") == 0){
		
			//send(clientSocket,input,1024,0);
			
			scanf("%s",input);
			
			int n = strlen(input) + 1024;
			//int m = strlen(input) + 20;
			char buff[55];
			
			char a[1024] = "smb://192.168.230.33/";
			char b[255] = "xdg-open ";
			char* link = concat_string(a, input);
			char* cmd = concat_string(b, link);
			
			int j = snprintf(buff, n, "%s\n", cmd);
			
			system(buff);
			printf("Done!\n\n");
		}
		if(strcmp(input,"TRANSFER") == 0){
		
			//send(clientSocket,input,1024,0);
			
			scanf("%s",input1);
			//send(clientSocket,input1,1024,0);
			
			scanf("%s",input2);
			//send(clientSocket,input2,1024,0);
			char a[1024] = " /run/user/1000/gvfs/smb-share:server=192.168.230.33,share=";
			char b[255] = "cp ";
			char buff[255];
			
			int n = strlen(input1) + strlen(input2) + 1024;
			
			char* link = concat_string(a, input2);
			char* cmd = concat_string(input1, a);
			char* cm = concat_string(b, cmd);
			
			int j = snprintf(buff, n, "%s\n", cm);
			
			if(system(buff) == 0){
				printf("File transfered!\n");
				printf("Link: %s\n\n", link);
				continue;
			}
			
			printf("Done!\n\n");
			
		}
		if(strcmp(input,"FORWARD") == 0){
			send(clientSocket,input,1024,0);
			
			scanf("%s",input1);
			send(clientSocket,input1,1024,0);
			
			scanf("%s",input2);
			send(clientSocket,input2,1024,0);
		} 

	}


}
