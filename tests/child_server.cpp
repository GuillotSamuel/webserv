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

#define SERVADDR_INFO struct sockaddr

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

void ft_error(std::string str)
{
	std::cerr << "Error: " << str << std::endl;
	exit(EXIT_FAILURE);
}

std::string ft_get_file_content(const std::string &path)
{
	std::ifstream file(path.c_str());

	if (!file.is_open())
	{
		ft_error("get file content failed");
	}

	std::ostringstream oss;
	oss << file.rdbuf();

	return (oss.str());
}



void handle_client(int connfd, t_socket socket_s)
{
	char received_line[4096];
	char socket_buffer[4096];

	memset(received_line, 0, sizeof(received_line));

	if (read(connfd, received_line, sizeof(received_line) - 1) < 0)
	{
		close(connfd);
		ft_error("connfd read failed");
	}

	std::string	received_line_cpy(received_line);
	size_t		method_end = received_line_cpy.find(' ');
	std::string	method;
	std::string	path;

	// std::cout << "\n\nTEST 1: " << received_line_cpy << "\n\n" << std::endl; // TEST

	if (method_end != std::string::npos)
	{
		method = received_line_cpy.substr(0, method_end);
	}
	else
	{
		close(connfd);
		ft_error("method_end failed");
	}


	/* ----- PATH PARSE ----- */

	size_t		path_start = received_line_cpy.find('/');

	if (path_start == std::string::npos)
	{
		close(connfd);
		ft_error("path_start failed");
	}

	size_t		path_end = (received_line_cpy).find(' ', method_end + 1);

	if (path_end != std::string::npos)
	{
		path = received_line_cpy.substr(path_start, (path_end - path_start));
	}
	else
	{
		close(connfd);
		ft_error("path_end failed");
	}


	/* ----- PATH CHECK ----- */

	if (path == "/")
	{
		path = "index.html";
	}
	else
	{
		size_t		path_html_start = path.find(".hmtl");

		if (path_html_start == std::string::npos)
		{
			close(connfd);
			ft_error("path_html_start failed");
		}

		size_t		path_html_end = path.find('\0', path_html_start);

		if (path_html_end != std::string::npos)
		{
			std::string	path_html = path.substr(path_html_start, (path_html_end - path_html_start));
			if (path_html.size() != 5)
			{
				close(connfd);
				exit ;
			}
		}
		else
		{
			close(connfd);
			ft_error("path_html_end failed");
		}
	}


	/* ----- METHOD ----- */

	std::cout << "\n\nTEST : " << path << "\n\n" << std::endl;

	if (method == "GET")
	{
		std::string get_content;
		get_content = ft_get_file_content("../www/html" + path);
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
		std::string error_content = ft_get_file_content("../www/html/errors/400.html");
		snprintf(socket_buffer, sizeof(socket_buffer),
				 "HTTP/1.0 400 Bad Request\r\n\r\n%s",
				 error_content.c_str());
	}

	write(connfd, socket_buffer, strlen(socket_buffer));

	close(connfd);
}

int main(int argc, char **argv)
{
	t_socket socket_s;

	socket_s.port = argv[1];
	if (argc != 2)
		ft_error("wrong arguments");

	if ((socket_s.sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		ft_error("socket creation failed");

	memset(&(socket_s.servaddr), 0, sizeof(socket_s.servaddr));

	socket_s.servaddr.sin_family = AF_INET;
	socket_s.servaddr.sin_port = htons(std::atoi(socket_s.port));
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
			close(socket_s.sockfd);
			handle_client(socket_s.connfd, socket_s);
			exit(0);
		}
		else
		{
			close(socket_s.connfd);
			waitpid(-1, NULL, WNOHANG);
		}
	}

	close(socket_s.sockfd);

	return (0);
}
