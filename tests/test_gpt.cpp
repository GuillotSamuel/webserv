#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>

#define SERVER_PORT 80
#define MAXLINE 4096
#define SA struct sockaddr

void err_n_die(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    if (errno) {
        fprintf(stderr, ": %s\n", strerror(errno));
    } else {
        fprintf(stderr, "\n");
    }

    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    int sockfd, n;
    int sendbytes;
    struct addrinfo hints, *res, *p;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE];
    char recvline[MAXLINE];

    // Check command line arguments
    if (argc != 2)
        err_n_die("usage: %s <server address>\n", argv[0]);

    // Prepare hints for getaddrinfo
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6
    hints.ai_socktype = SOCK_STREAM;

    // Resolve the server address and port
    if (getaddrinfo(argv[1], NULL, &hints, &res) != 0)
        err_n_die("getaddrinfo error for %s", argv[1]);

    // Iterate over the list of resolved addresses
    for (p = res; p != NULL; p = p->ai_next) {
        if (p->ai_family == AF_INET) {
            // Copy the resolved address into the sockaddr_in struct
            servaddr = *(struct sockaddr_in *)p->ai_addr;
            servaddr.sin_port = htons(SERVER_PORT);

            // Create a socket
            if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                err_n_die("Error while creating the socket");

            // Connect to the server
            if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0) {
                close(sockfd); // Close socket on failure
                continue; // Try the next address
            }
            break; // Successfully connected
        }
    }

    freeaddrinfo(res); // Free the list of addresses

    if (p == NULL)
        err_n_die("Unable to connect to any address for %s", argv[1]);

    snprintf(sendline, sizeof(sendline),
             "GET / HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n"
             "\r\n", argv[1]);
    sendbytes = strlen(sendline);

    if (write(sockfd, sendline, sendbytes) != sendbytes)
        err_n_die("write error");

    while ((n = read(sockfd, recvline, MAXLINE - 1)) > 0)
    {
        recvline[n] = '\0';
        printf("%s", recvline);
    }

    if (n < 0)
        err_n_die("read error");

    close(sockfd);

    exit(EXIT_SUCCESS);
}
