#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MAXLINE 4096   /*max text line length*/
#define SERV_PORT 9091 /*port*/
#define LISTENQ 8      /*maximum number of client connections */

typedef struct guestList
{
  struct sockaddr_in cliaddr;
  int client;
  struct guestList *next;
} guestList;

static int *guests;
int main(int argc, char **argv)
{
  guests = mmap(NULL, 2 * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  guests[0] = guests[1] = 0;
  int listenfd, connfd, n;
  int client;
  pid_t childpid;
  socklen_t clilen;
  char buf[MAXLINE];
  struct sockaddr_in cliaddr, servaddr;

  //creation of the socket
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  //preparation of the socket address
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);

  if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) >= 0)
  {
    puts("Server address is maybe:");
    system("ifconfig | perl -nle'/dr:(\\S+)/ && print $1'");
    printf("Server is running at port %d\n", SERV_PORT);
  }
  else
  {
    perror("bind failed");
    return 0;
  }

  
  for (;;)
  {
    puts("Line 55");
    listen(listenfd, LISTENQ);
  printf("%s\n", "Server running...waiting for connections.");
    clilen = sizeof(cliaddr);
    connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
      //close listening socket
      // close(listenfd);
      printf("Listing in %d\n", listenfd);
    fflush(stdout);
    if (fork() == 0)
    {
      //Child process
      fprintf(stdout, "Received request from %s:%d\n", inet_ntoa(cliaddr.sin_addr), htons(cliaddr.sin_port));
      fprintf(stdout, "%d", connfd);
      fflush(stdout);
      if (guests[0] == 0)
      {
        guests[0] = connfd;
        client = 0;
        send(connfd, "you are 1", 10, 0);
        printf("linegues");
        fflush(stdout);
      }
      else
      {
        guests[1] = connfd;
        client = 1;
        send(connfd, "you are 2", 10, 0);
        printf("minegues");
        fflush(stdout);
      }

      while ((n = recv(connfd, buf, MAXLINE, 0)) > 0)
      {
        char *response = buf;
        response[n] = 0;
        printf("%s", response);
        int responseLength = strlen(response);
        if (guests[1 - client] != 0)
        {
          // fprintf(stdout, "send to %d:", guests[1-client]);
          send(guests[client], "whhw", 4, 0);
          send(guests[1 - client], response, 8, 0);
          printf("Send to %d\n", guests[1-client]);
        }
        else
        {
          send(guests[client], "Wait for other user!", 21, 0);
        }
      }

      if (n < 0)
      {
        perror("Read error");
        exit(1);
      }
      close(connfd);
    }
  }
}