#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 3000 /*port*/
#define LISTENQ 8 /*maximum number of client connections */

void createServer(void (*callback)(char*, char*))
{
 int listenfd, connfd, n;
 pid_t childpid;
 socklen_t clilen;
 char buf[MAXLINE];
 struct sockaddr_in cliaddr, servaddr;
	
 //creation of the socket
 listenfd = socket (AF_INET, SOCK_STREAM, 0);	
 //preparation of the socket address 
 servaddr.sin_family = AF_INET;
 servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
 servaddr.sin_port = htons(SERV_PORT);
	
 bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	
 listen (listenfd, LISTENQ);
	
 printf("%s\n","Server running...waiting for connections.");
	
 for ( ; ; ) {
		
  clilen = sizeof(cliaddr);
  connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);
  printf("%s\n","Received request...");
				
  while ( (n = recv(connfd, buf, MAXLINE,0)) > 0)  {
   printf("%s","String received from and resent to the client:");
   buf[n] = 0; // Get actual request
   char response[MAXLINE];
   (*callback)(buf, response);
   int responseLength = strlen(response);
   send(connfd, response, responseLength, 0);
  }
			
 if (n < 0) {
  perror("Read error"); 
  exit(1);
 }
 close(connfd);
	
 }
 //close listening socket
 close (listenfd); 
}


void createClient(void (*callback)(char*, char*)) 
{
 int sockfd, responseLength;
 struct sockaddr_in servaddr;
 char sendline[MAXLINE], recvline[MAXLINE];
	
 //Create a socket for the client
 //If sockfd<0 there was an error in the creation of the socket
 if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
  perror("Problem in creating the socket");
  exit(2);
 }
	
 //Creation of the socket
 memset(&servaddr, 0, sizeof(servaddr));
 servaddr.sin_family = AF_INET;
 servaddr.sin_addr.s_addr= inet_addr("0.0.0.0");
 servaddr.sin_port =  htons(SERV_PORT); //convert to big-endian order
	
 //Connection of the client to the socket 
 if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
  perror("Problem in connecting to the server");
  exit(3);
 }
	
 while (fgets(sendline, MAXLINE, stdin) != NULL) {
	
  send(sockfd, sendline, strlen(sendline), 0);
		
  if ((responseLength = recv(sockfd, recvline, MAXLINE,0)) == 0){
   //error: server terminated prematurely
   perror("The server terminated prematurely"); 
   exit(4);
  }
  recvline[responseLength] = 0; //Get the actual response value
  (*callback)(sendline, recvline);
 }

 exit(0);
}