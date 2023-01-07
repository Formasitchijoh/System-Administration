#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"
static const int BUFSIZE = 1024;
static const int MAXPENDING = 5; //Maximum outstanding connection reuests
void HandleTCPClient(int clntSocket);
int main(int argc, char *argv[]){

    if(argc != 2){
        DieWithUserMessage("Parameter(s)", "<erver Port>");
    }

    in_port_t servPort = atoi(argv[1]); //first argument is the local port

    //create socket for incomming connections

    int servSock;

    if((servSock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0)
        DieWithSystemMessage("socket() failed");


        //constructing a local addres atructure

        struct sockaddr_in servAddr;
        memset(&servAddr, 0 , sizeof(servAddr));
        servAddr.sin_family = AF_INET;
        servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servAddr.sin_port = htons(servPort);


        //Bind to local addres

        if(bind(servSock, (struct sockaddr*)&servAddr,sizeof(servAddr)) < 0)
            DieWithSystemMessage("bind() failed");

            //make the socket to listen to incomm=ming connections
            if(listen(servSock, MAXPENDING) < 0)
                DieWithSystemMessage("listen() failed");

                
                for(;;){

                    struct sockaddr_in clntAddr; //Client address that conatsins the element of a socket address

                    //etting the length of the client address structure(in-out-parameter)
                    socklen_t clntAddrlen = sizeof(clntAddr);

                    //wait for a client to connect

                    int clntSock = accept(servSock, (struct sockaddr*)&clntAddr, &clntAddrlen);
                    if(clntSock < 0){
                        DieWithSystemMessage("accept() failed");
                    }

                    //clntSock is connected to a client

                    char clntName[INET_ADDRSTRLEN];  //string to containt client address
                    if(inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName, sizeof(clntName)) != NULL)
                        printf("Handling client %s/%d\n", clntName, ntohl(clntAddr.sin_port));
                    else 
                        puts("Unable to get client address");

                        HandleTCPClient(clntSock);
                }

}
 void HandleTCPClient(int clntSocket){
                    char buffer[BUFSIZE]; 

                    //rECEIVE MEAGE FROM CLIENT
                ssize_t numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
                if(numBytesRcvd < 0){
                    DieWithSystemMessage("ecv failed");
                }

                //end received string and receive again until end of stream

                while (numBytesRcvd > 0){
                    //echoing the message back to the client

                    ssize_t numBytesSent = send(clntSocket, buffer, numBytesRcvd, 0);
                    if(numBytesSent < 0)
                        DieWithSystemMessage("send() failed");
                    else if(numBytesSent != numBytesRcvd)
                        DieWithUserMessage("send()", "sent unexpected number of bytes");


                        //see if there is more data to receive
                      //Receivint BUFSIZE byte of  data into buffer from the cnltSocket with a flag of 0
                        numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
                        if(numBytesRcvd < 0){
                            DieWithSystemMessage("recv() failed");
                        }
                        close(clntSocket);
                    
                }
                {
                    /* code */
                }
                
                }