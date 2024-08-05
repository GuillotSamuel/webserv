#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVADDR_INFO struct sockaddr

typedef struct s_socket
{
	int sockfd;
	int connfd;
	struct sockaddr_in servaddr;
	char *receiveline;
	char *socket_buffer;
	int n;
} t_socket;

void ft_error(std::string str)
{
	std::cerr << "Error: " << str << std::endl;
}

int main(int argc, char **argv)
{
	t_socket socket_s;

	if (argc != 2)
		ft_error("wrong arguments");

	if ((socket_s.sockfd = socket(AF_INET, SOCK_STREAM, 0) < 0))
		ft_error("socket creation failed");

	bzero(&(socket_s.servaddr), sizeof(socket_s.servaddr));

	socket_s.servaddr.sin_family = AF_INET;
	socket_s.servaddr.sin_port = htons(std::atoi(argv[1]));
	socket_s.servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (inet_pton(AF_INET, argv[1], &(socket_s.servaddr.sin_addr)) <= 0)
		ft_error("inet_pton failed");
	if (connect(socket_s.sockfd, (SERVADDR_INFO *)&socket_s.servaddr, sizeof(socket_s.servaddr)) < 0)
		ft_error("connect error");

	if (bind(socket_s.sockfd, (SERVADDR_INFO *)&socket_s.servaddr, sizeof(socket_s.servaddr)) < 0)
		ft_error("bind error");
	if (listen(socket_s.sockfd, 10) < 0)
		ft_error("listen error");

	while (1)
	{
		socket_s.connfd = accept(socket_s.sockfd, (SERVADDR_INFO *)NULL, NULL);
	}

	memset(socket_s.receiveline, 0, 4096);

	while (socket_s.n = read(socket_s.connfd, socket_s.receiveline, 4095))
	{
		if (socket_s.receiveline[socket_s.n - 1] == '\n')
			break;
	}

	snprintf(socket_s.socket_buffer, sizeof(socket_s.socket_buffer), "HTTP/1.0 200 OK\r\n\r\nHello");

	write(socket_s.connfd, socket_s.socket_buffer, strlen(socket_s.socket_buffer));

	close(socket_s.connfd);
}
