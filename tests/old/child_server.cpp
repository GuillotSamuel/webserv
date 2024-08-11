#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
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
#include <sys/epoll.h>
#define SERVADDR_INFO struct sockaddr
#define MAX_EVENTS 10

/*
./http_server 8080
curl -X GET http://localhost:8080
curl -X POST http://localhost:8080
curl -X DELETE http://localhost:8080
 */

typedef struct s_socket
{
	int sockfd;
	int connfd;
	char *port;
	struct sockaddr_in servaddr;
	char receiveline[4096];
	char socket_buffer[4096];
	int n;
} t_socket;

// void ft_error(std::string str, int connfd)
// {
// 	if (connfd != -1)
// 		close(connfd);
// 	std::cerr << "Error: " << str << std::endl;
// 	exit(EXIT_FAILURE);
// }
std::string ft_get_file_content(const std::string &path)
{
	std::ifstream file(path.c_str());

	if (!file.is_open())
	{
		// ft_error("get file content failed", -1);
		return (ft_get_file_content("../www/html/errors/400.html"));
	}

	std::ostringstream oss;
	oss << file.rdbuf();
	return (oss.str());
}

std::string	findContent(t_socket socket_s, std::string receivedLine)
{
	std::string	path;
	size_t		path_start;
	size_t		path_end;

	path_start = receivedLine.find('/');
	if (path_start == std::string::npos)
		ft_error("path_start failed", socket_s.connfd);
	path_end = (receivedLine).find(' ', path_start);
	if (path_end == std::string::npos)
		ft_error("path_end failed", socket_s.connfd);
	else
	{
		path = receivedLine.substr(path_start, (path_end - path_start));
	}

	/* ----- PATH CHECK ----- */

	if (path == "/")
		return (ft_get_file_content("../www/html/index.html"));
	else
	{
		if (path.compare(path.size()-5, 5, ".html") == 0 ) // Le path finis par .html
		{
			std::cout << path << std::endl; //TEST
			return (ft_get_file_content("../www/html" + path));
		}
		else if (path.compare(path.size()-3, 3, ".css") == 0 )
		{
			return (ft_get_file_content("../www/styles" + path));
		}
	}
	return (ft_get_file_content("../www/html/errors/400.html"));
}

void handle_client(int connfd, t_socket socket_s)
{
	char		received_line[4096];
	char		socket_buffer[4096];

	memset(received_line, 0, sizeof(received_line));

	if (read(connfd, received_line, sizeof(received_line) - 1) < 0)
		ft_error("connfd read failed", connfd);

	std::string	received_line_cpy(received_line);
	size_t		method_end = received_line_cpy.find(' ');
	std::string	method;
	std::string	path;

	std::cout << "\n\nTEST 1: " << received_line_cpy << "\n\n" << std::endl; // TEST

	if (method_end != std::string::npos)
		method = received_line_cpy.substr(0, method_end);
	else
		ft_error("method_end failed", connfd);


	/* ----- PATH PARSE ----- */

	std::string get_content = findContent(socket_s, received_line);
	
	/* ----- METHOD ----- */


	std::cout << "\n\nTEST : -" << get_content << "-\n\n" << std::endl;

	if (method == "GET")
	{
		snprintf(socket_buffer, sizeof(socket_buffer),
				 "HTTP/1.0 200 OK\r\n\r\n%s",
				 get_content.c_str());
	}
	else if (method == "POST")
	{
		snprintf(socket_buffer, sizeof(socket_buffer),
				 "HTTP/1.0 200 OK\r\n\r\n"
				 "Received POST request\n");
	}
	else if (method == "DELETE")
	{
		snprintf(socket_buffer, sizeof(socket_buffer),
				 "HTTP/1.0 200 OK\r\n\r\n"
				 "Received DELETE request\n");
	}
	else
	{
		snprintf(socket_buffer, sizeof(socket_buffer),
				 "HTTP/1.0 400 Bad Request\r\n\r\n%s",
				 get_content.c_str());
	}
	write(connfd, socket_buffer, strlen(socket_buffer));
	close(connfd);
}

// int main(int argc, char **argv)
{
	// t_socket socket_s;

	// socket_s.port = argv[1];
	// if (argc != 2)
	// 	ft_error("wrong arguments", -1);

	// if ((socket_s.sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	// 	ft_error("socket creation failed", -1);

	// memset(&(socket_s.servaddr), 0, sizeof(socket_s.servaddr));

	// socket_s.servaddr.sin_family = AF_INET;
	// socket_s.servaddr.sin_port = htons(std::atoi(socket_s.port));
	// socket_s.servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	/* 	if (inet_pton(AF_INET, argv[1], &(socket_s.servaddr.sin_addr)) <= 0)
			ft_error("inet_pton failed");
		if (connect(socket_s.sockfd, (SERVADDR_INFO *)&socket_s.servaddr, sizeof(socket_s.servaddr)) < 0)
			ft_error("connect error"); */

	// if (bind(socket_s.sockfd, (SERVADDR_INFO *)&socket_s.servaddr, sizeof(socket_s.servaddr)) < 0)
	// 	ft_error("bind error", -1);
	// if (listen(socket_s.sockfd, 10) < 0)
	// 	ft_error("listen error", -1);

	// int	epoll_fd = epoll_create(1);
	// if (epoll_fd == -1)
	// 	ft_error("epoll_create1 failed", -1);

	// struct epoll_event	ev;
	// ev.events = EPOLLIN;
	// ev.data.fd = socket_s.sockfd;
	
	// if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_s.sockfd, &ev) == -1)
	// 	ft_error("epoll_ctl failed", -1);
	
	// struct epoll_event events[MAX_EVENTS];

	// while (true)
	// {
	// 	int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
	// 	if (nfds == -1)
	// 		ft_error("epoll_wait, failed", -1);
	// 	for (int i = 0; i < nfds; ++i)
	// 	{
	// 		if (events[i].data.fd == socket_s.sockfd)
	// 		{
	// 			socket_s.connfd = accept(socket_s.sockfd, NULL, NULL);
	// 			if (socket_s.connfd == -1)
	// 				ft_error("accept failed", -1);

	// 			ev.events = EPOLLIN | EPOLLET;
	// 			ev.data.fd = socket_s.connfd;

	// 			if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_s.connfd, &ev) == -1)
	// 				ft_error("epoll_ctl (1) failed", socket_s.connfd);
	// 		}
	// 		else
	// 		{
	// 			handle_client(events[i].data.fd, socket_s);
				// if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL) == -1)
				// 	ft_error("epoll_ctl (2) failed", socket_s.connfd); // failed a chaque appel
	// 		}
	// 	}
	// }

	// close(socket_s.sockfd);
	// close(epoll_fd);
	return (0);
}
