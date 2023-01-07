#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"

static const int BUFFSIZE = 20;
int main(int argc, char *argv[]){

    if(argc < 3 || argc  > 4) //Testing for correct number of arguments
        DieWithUserMessage("Parameter(s)","<Server Address> <Echo Word> [<Server Port]");


        char *servIP = "192.168.8.100"; //First arg: server IP address
        char *echoString = argv[2]; //send arg: string to echo

        //checking for the thrid string which is optional 7 is well-known wcho port

        in_port_t servPOrt = (argc == 4) ? atoi(argv[3]) : 7;

        //create a reliable stream socket using TCP

        int sock = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP); //CREATED A TCP SOCKET

        if(sock < 0) {
            DieWithSystemMessage("socket() failed");
        }

        //else construct the tcp server 

        struct sockaddr_in servAddr;
        memset(&servAddr, 0 , sizeof(servAddr));
        servAddr.sin_family = AF_INET;

        //Convert Adress

        int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);

        if(rtnVal == 0){
            DieWithUserMessage("inet_pton() failed", "invalid address string");
        } else if(rtnVal < 0)
            DieWithSystemMessage("inet_pton() failed");
    
    servAddr.sin_port = htons(servPOrt); //server port
    //establih echotringLen 

            if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
              DieWithSystemMessage("connect() failed");
              
              size_t echoStringLen = strlen(echoString); //Determine input length
              
              //send the strring to the server
              ssize_t numBytes = send(sock, echoString, echoStringLen, 0);

              if(numBytes < 0){
                DieWithSystemMessage("send() failed");
              }else if(numBytes != echoStringLen)
                {
                    DieWithUserMessage("send()", "sent unexpected number of bytes");
                }

                //Receiving the same tring back from the server

                unsigned int totallBytesRcvd = 0;
                fputs("Received:", stdout);  //setup to print the echoed string

                while(totallBytesRcvd < echoStringLen){
                    char buffer[BUFFSIZE];  //I/O buffer
                    //Receiving up to the buffer Size minu 1 to leave space for a null terminator

                    numBytes = recv(sock, buffer, BUFFSIZE -1, 0);
                    if(numBytes < 0)
                        DieWithSystemMessage("recv failed");
                    else if(numBytes ==0 )
                        DieWithUserMessage("recv()","connection closed prematurely");

                        totallBytesRcvd += numBytes;
                        buffer[numBytes] = '\0';
                        fputs(buffer, stdout);
                }
                fputc('\n', stdout); //Print a final linefeed
                close(sock);
                exit(0);

                

}