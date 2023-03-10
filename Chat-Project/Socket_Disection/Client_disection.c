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
#include <errno.h>
#define PORT_NUMBER     5000
#define SERVER_ADDRESS  "192.168.8.101"
static const int BUFFSIZE = 20;
#define FILENAME        "/home/prudence/foo.c"

int main(int argc, char **argv){
	
        ssize_t len;
        struct sockaddr_in server_addr;
        char buffer[BUFSIZ];
        int file_size;
        FILE *received_file;
        int remain_data = 0;

	memset(&server_addr, 0, sizeof(server_addr));

     //construct server_addr struct
	 server_addr.sin_family = AF_INET;
	 inet_pton(AF_INET, SERVER_ADDRESS,&(server_addr.sin_addr));
	 server_addr.sin_port = htons(PORT_NUMBER);

	 //Creating client socket

	 int Client_sock = socket(AF_INET, SOCK_STREAM,0);

	 //checking if the socket is created

	 if(Client_sock < 0){
             fprintf(stderr, "Error creating socket  ----> %s\n", strerror(errno));
			 exit(EXIT_FAILURE);
	 }

	 //Connecting to the server

	 int connectResult = connect(Client_sock,(struct sockaddr*)&server_addr,sizeof(struct sockaddr));
	 if(connectResult < 0){
		fprintf(stderr, "Error connecting to server --> %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	 }

	 //After connecting we start receiving files from the socket
	 recv(Client_sock, buffer, BUFSIZ,0);
	 file_size = atoi(buffer);

	 received_file = fopen(FILENAME , "w");
	 if(received_file == NULL){
		fprintf(stderr, "Failed to open file --> %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	 }
	 remain_data = file_size;


	 //continuously write the data intoa file in the client file

	 while((remain_data > 0) && ((len = recv(Client_sock, buffer, BUFFSIZE,0)) > 0)){
		fwrite(buffer, sizeof(char), len, received_file);
		remain_data -= len;
		fprintf(stdout, "Received %ld bytes and we hope : %d bytes\n", len, remain_data);

	 }
	 fclose(received_file);
	 close(Client_sock);

}




































































// int clientCount = 0;

// static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// struct client{

// 	int index;
// 	int sockID;
// 	struct sockaddr_in clientAddr;
// 	int len;

// };

// struct client Client[1024];
// pthread_t thread[1024];

// void * doNetworking(void * ClientDetail){

// 	struct client* clientDetail = (struct client*) ClientDetail;
// 	int index = clientDetail -> index;
// 	int clientSocket = clientDetail -> sockID;

// 	printf("Client %d connected.\n",index + 1);

// 	while(1){

// //the clientSocket here is the new ocket that has been created for each client that connects to the server socket
// //hence when a new client connects to a server a new coket is creted for it which will be used for sending or receiving messages from or to it and the server ocket is still kept is still kept open for new connections
// 		char data[1024];
// 		int read = recv(clientSocket,data,1024,0);
// 		data[read] = '\0';

// 		char output[1024];

// 		if(strcmp(data,"LIST") == 0){

// 			int l = 0;

// 			for(int i = 0 ; i < clientCount ; i ++){

// 				if(i != index)
// 					l += snprintf(output + l,1024,"Client %d is at socket %d.\n",i + 1,Client[i].sockID);

// 			}

// 			send(clientSocket,output,1024,0);
// 			continue;

// 		}
// 		if(strcmp(data,"SEND") == 0){

// 			read = recv(clientSocket,data,1024,0);
// 			data[read] = '\0';

// 			int id = atoi(data) - 1;

// 			read = recv(clientSocket,data,1024,0);
// 			data[read] = '\0';

// 			send(Client[id].sockID,data,1024,0);			

// 		}

// 	}

// 	return NULL;

// }
// //  https://www.quora.com/Project-ideas-based-on-socket-programming-in-java-other-than-chat-application
// int main(){
   
//    /*
//    Socket Creation:
//    int sockid = socket(family, type, protocol)
//    family: PF_INET,IPV4 protocols, Internet addresses(typically used) or PF_UNIX

//    type: communication type 
//    SOCK_STREAM reliable 2-way connection
//    SOCK_DGRAM unreliable, conectionle, message of max length
   
//    protocol:specifies protocol
//    IPPROTO_TCP/UDP  commonly set to 0
//    */
// 	int serverSocket = socket(PF_INET, SOCK_STREAM, 0);     //this created socket is also called socket descriptor

// /*
// socket API defines a generic datatype for addresses:

// The Generic type are 

// struct sockaddr {
//     unsigned short sa_family;
//     char sa_data[14];
// }
// particular socket API for TCP/IP addresses

// struct in_addr {
//     unsigned long s_addr;  Internet address
// }

// struct sockaddr_in {
//     unsigned short sin_family   internet protocol(AF_INET)
//     unsigned short sin_port    Adress port(16bit)
//     struct in_addr sin_addr   Internet address(32bit)
//     char sin_zero[8]  NOT USED
// }

// '''''''''''''''' sockaddr_in can be casted to a sockaddr
// */
// 	struct sockaddr_in serverAddr;    //API that contains the components that make up a socket

// 	serverAddr.sin_family = AF_INET;
// 	serverAddr.sin_port = htons(8080);
// 	serverAddr.sin_addr.s_addr = inet_addr("192.168.43.204");


// 	if(bind(serverSocket,(struct sockaddr *) &serverAddr , sizeof(serverAddr)) == -1) return 0;

// /*
// listen takes 2 parameters socketd  descriptor and ueuelen ie the number of 
// active participants that can wait for a connection in this case thers are 1024
// */
// 	if(listen(serverSocket,1024) == -1) 
// 	return 0;
	
// 	system("chmod +x smb.sh");
// 	system("ls /home/prudence/Documents");
// 	system("./smb.sh");
// 	printf("Server started listenting on port 8080 ...........\n");

// 	while(1){
//          /*
//           The server gets or a new socket is created for each incoming connection by calling the accept() method
//            the return value of a after a socket is created is an integer

//           int s =  accept(sockid, &clientaddr, &addrLen);
//           s : integer the new socket (used for data tranfer)
//           sockid:integer the original socket being listebed to /sockey descriptor
//           clientAddr: struct sockaddr address of the active participants
//           addrLen:sizeof(clientAddr) value/result parameter
//            this must be set appropriately before call and adjusted upon return
//          */

// 		Client[clientCount].sockID = accept(serverSocket, (struct sockaddr*) &Client[clientCount].clientAddr, &Client[clientCount].len);
// 		Client[clientCount].index = clientCount;

// 		pthread_create(&thread[clientCount], NULL, doNetworking, (void *) &Client[clientCount]);

// 		clientCount ++;
 
// 	}

// 	for(int i = 0 ; i < clientCount ; i ++)
// 		pthread_join(thread[i],NULL);

// }
