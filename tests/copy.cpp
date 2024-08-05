#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SERVER_PORT 18000
#define AAA struct sockaddr 

int main(void)
{
    int     sockfd;
    int     connfd;
    int     n;
    struct sockaddr_in  servaddr;

    char    *receiveline;
    char    *buff;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT); // sur le port qu'on ecoute
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // responding to anything

    bind(sockfd, (AAA *)&servaddr, sizeof(servaddr)); //this socket is goind to listen to this adress
    listen(sockfd, 10); // we are listening
    while (1)
    {
        connfd = accept(sockfd, (AAA *)NULL, NULL); //accept every connection
    }
    memset(receiveline, 0, 4096);
    while (n = read(connfd, receiveline, 4095) > 0)
    {
        if (receiveline[n-1] == '\n')
            break;
    }
    snprintf(buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\nHello");
    write(connfd, buff, strlen(buff));
    close(connfd);
}