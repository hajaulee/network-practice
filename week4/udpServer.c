#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#define SERV_PORT 1255
#define MAXLINE 255

int main()
{
    int sockfd, n;
    socklen_t len;
    char mesg[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;
    struct sockaddr_in guests[10];
    int guest_count = 0;
    char name[2][30];
    char sms[500];
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
    while (guest_count != 2)
    {
        n = recvfrom(sockfd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
        mesg[n] = 0;
        if (guest_count == 0)
        {
            guests[0] = cliaddr;
            strcpy(name[0], mesg);
            guest_count = 1;
        }
        if (memcmp(&cliaddr, &guests[0], sizeof(struct sockaddr_in)) != 0)
        {
            guests[1] = cliaddr;
            strcpy(name[1], mesg);
            guest_count = 2;
        }
        else
            sendto(sockfd, "Wait for user", 13, 0, (struct sockaddr *)&cliaddr, len);
    }
    for (;;)
    {
        len = sizeof(cliaddr);
        puts("Receiving data ...");
        n = recvfrom(sockfd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
        mesg[n] = 0;
        puts(mesg);
        printf("Sending data : %s", mesg);
        struct sockaddr_in send_to = guests[0];
        int index = 0;
        if (memcmp(&cliaddr, &guests[0], sizeof(struct sockaddr_in)) == 0){
            send_to = guests[1];
            index = 1;
        }
        strcat(strcat(strcpy(sms, name[1-index]),": "),  mesg);
        sendto(sockfd, sms, strlen(sms), 0, (struct sockaddr *)&send_to, len);
    }
    close(sockfd);
    return 0;
}