#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#define SERV_PORT 1255
#define MAXLINE 255
int main()
{
    int sockfd, n;
    socklen_t len;
    char mesg[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) >= 0)
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
        len = sizeof(cliaddr);
        puts("Receiving data ...");
        n = recvfrom(sockfd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
        mesg[n]= 0; // Get real request
        printf("Recivied data from %s:%d>>>>", inet_ntoa(cliaddr.sin_addr), htons(cliaddr.sin_port));
        puts(mesg);
        printf("Sending data : %s\n", mesg);
        sendto(sockfd, mesg, n, 0, (struct sockaddr *)&cliaddr, len);
    }
    close(sockfd);
    return 0;
}