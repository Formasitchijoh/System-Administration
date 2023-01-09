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

int clientCount = 0;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

struct client{

	int index;
	int sockID;
	struct sockaddr_in clientAddr;
	int len;

};

struct client Client[1024];
pthread_t thread[1024];

char* concat_string(char s[], char s1[]){
	int i;
	int j = strlen(s);
	for(i=0; s1[i] != '\0'; i++){
		s[i+j] = s1[i];
	}
	
	s[i+j] = '\0';
	
	return s;
}

void * doNetworking(void * ClientDetail){

	struct client* clientDetail = (struct client*) ClientDetail;
	int index = clientDetail -> index;
	int clientSocket = clientDetail -> sockID;

	printf("Client %d connected.\n",index + 1);

	while(1){

		char data[1024];
		char data1[1024];
		char buff[1024];
			
		char a[1024] = " /home/yhoung-cute/public/";
		char b[255] = "cp";
			
		int read = recv(clientSocket,data,1024,0);
		data[read] = '\0';

		char output[1024];

		if(strcmp(data,"LIST") == 0){

			int l = 0;

			for(int i = 0 ; i < clientCount ; i ++){

				if(i != index)
					l += snprintf(output + l,1024,"Client %d is at socket %d.\n",i + 1,Client[i].sockID);

			}

			send(clientSocket,output,1024,0);
			continue;

		}
		if(strcmp(data,"SEND") == 0){

			read = recv(clientSocket,data,1024,0);
			data[read] = '\0';

			int id = atoi(data) - 1;

			read = recv(clientSocket,data,1024,0);
			data[read] = '\0';

			send(Client[id].sockID,data,1024,0);			

		}
		/*if(strcmp(data,"COPY") == 0){
			int l = 0;
			char output[1024];
			char output1[1024];
		
			read = recv(clientSocket,data,1024,0);
			data[read] = '\0';
			
			read = recv(clientSocket,data1,1024,0);
			data1[read] = '\0';
			
			int n = strlen(data) + strlen(data1) + 30;
			
			char* link = concat_string(data, data1);
			char* cmd = concat_string(link, a);
			char* cm = concat_string(b, cmd);
			
			int j = snprintf(buff, n, "%s\n", cm);
			
			if(system(buff) == 0){
				printf("Link: %s\n", link);
				l += snprintf(output + l,1024,"File copied!\n");
				send(clientSocket,output,1024,0);
				
				continue;
			}
		}*/
		if(strcmp(data,"FORWARD") == 0){
			int l = 0;
			char output[1024];
			char output1[1024];
			char c[1024] = " /home/yhoung-cute/";
			char d[1024] = " /home/yhoung-cute/";
		
			read = recv(clientSocket,data,1024,0);
			data[read] = '\0';
			
			read = recv(clientSocket,data1,1024,0);
			data1[read] = '\0';
			
			int n = strlen(data) + strlen(data1) + 1024;
			
			char* link = concat_string(c, data1);
			char* wow = concat_string(d, data);
			char* cmd = concat_string(wow, link);
			char* cm = concat_string(b, cmd);
			
			int j = snprintf(buff, n, "%s\n", cm);
			
			if(system(buff) == 0){
				printf("Link: %s\n", link);
				l += snprintf(output + l,1024,"File forwarded!\nDone!\n");
				send(clientSocket,output,1024,0);
				
				continue;
			}
		}

	}

	return NULL;

}

int main(){

	int serverSocket = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serverAddr;

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8080);
	serverAddr.sin_addr.s_addr = inet_addr("192.168.230.33");


	if(bind(serverSocket,(struct sockaddr *) &serverAddr , sizeof(serverAddr)) == -1) return 0;

	if(listen(serverSocket,1024) == -1) 
	return 0;
	
	system("chmod +x smb.sh");
	system("./smb.sh");
	printf("Server started listenting on port 8080 ...........\n");

	while(1){

		Client[clientCount].sockID = accept(serverSocket, (struct sockaddr*) &Client[clientCount].clientAddr, &Client[clientCount].len);
		Client[clientCount].index = clientCount;

		pthread_create(&thread[clientCount], NULL, doNetworking, (void *) &Client[clientCount]);

		clientCount ++;
 
	}

	for(int i = 0 ; i < clientCount ; i ++)
		pthread_join(thread[i],NULL);

}
