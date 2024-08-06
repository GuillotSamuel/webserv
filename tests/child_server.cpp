#include <iostream>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <poll.h>  

#define SERVADDR_INFO struct sockaddr

typedef struct s_socket
{
	int		sockfd;
	int		connfd;
	struct	sockaddr_in servaddr;
	char	receiveline[4096];
	char	socket_buffer[4096];
	int		n;
    struct  pollfd mypoll;
}			t_socket;

void ft_error(std::string str)
{
	std::cerr << "Error: " << str << std::endl;
	exit(EXIT_FAILURE);
}

void handle_client(int connfd, t_socket socket_s)
{
    char receiveline[4096];
    char socket_buffer[4096];
    int n;

    memset(receiveline, 0, sizeof(receiveline));
	printf("-%s-\n", receiveline);
	if (poll(&socket_s.mypoll, 1, 100) == 1)
	{
		while ((n = read(connfd, receiveline, sizeof(receiveline) - 1)) > 0)
		{
			if (receiveline[n - 1] == '\n')
				break;
		}
	}
    snprintf(socket_buffer, sizeof(socket_buffer),
             "HTTP/1.0 200 OK\r\n\r\n"
             "Hello");

    write(connfd, socket_buffer, strlen(socket_buffer));
    close(connfd);
}

int main(int argc, char **argv)
{
	t_socket socket_s;

	if (argc != 2)
		ft_error("wrong arguments");

	memset(&socket_s.mypoll, 0, sizeof(socket_s.mypoll));
	if ((socket_s.sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		ft_error("socket creation failed");
	socket_s.mypoll.fd = socket_s.sockfd;
	socket_s.mypoll.events = POLLIN;
	memset(&(socket_s.servaddr), 0, sizeof(socket_s.servaddr));

	socket_s.servaddr.sin_family = AF_INET;
	socket_s.servaddr.sin_port = htons(std::atoi(argv[1]));
	socket_s.servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

/* 	if (inet_pton(AF_INET, argv[1], &(socket_s.servaddr.sin_addr)) <= 0)
		ft_error("inet_pton failed");
	if (connect(socket_s.sockfd, (SERVADDR_INFO *)&socket_s.servaddr, sizeof(socket_s.servaddr)) < 0)
		ft_error("connect error"); */

	if (bind(socket_s.sockfd, (SERVADDR_INFO *)&socket_s.servaddr, sizeof(socket_s.servaddr)) < 0)
		ft_error("bind error");
	if (listen(socket_s.sockfd, 10) < 0)
		ft_error("listen error");

	while (true)
    {

        if ((socket_s.connfd = accept(socket_s.sockfd, (SERVADDR_INFO *)NULL, NULL)) < 0)
            ft_error("accept error");

        pid_t pid = fork();
        if (pid < 0)
        {
            ft_error("fork error");
        }
        else if (pid == 0)
        {
            // Child process
            close(socket_s.sockfd);
            handle_client(socket_s.connfd, socket_s);
            exit(0);
        }
        else
        {
            // Parent process
            close(socket_s.connfd);
            waitpid(-1, NULL, WNOHANG);
        }
    }

	close(socket_s.sockfd);

	return (0);
}
