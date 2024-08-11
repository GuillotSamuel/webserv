#include "serverExecution.hpp"

ServerExecution::ServerExecution(char *port)
{
    this->_socket = new ListeningSocket(port);
	this->epoll_fd = epoll_create(1);

	if (this->epoll_fd == -1)
	{
		ft_error("epoll_create failed");
	}

	this->ev.events = EPOLLIN;
	this->ev.data.fd = _socket->getSockFd();

	if (epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, _socket->getSockFd(), &this->ev) == -1)
	{
		ft_error("epoll_ctl failed");
	}
	
	serverExecutionFunction();
}

ServerExecution::~ServerExecution()
{
	if (epoll_fd != -1)
	{
		close(epoll_fd);
	}
}

void	ServerExecution::serverExecutionFunction()
{
	while (true)
	{
		int nfds = epoll_wait(this->epoll_fd, this->events, MAX_EVENTS, -1);

		if (nfds == -1)
		{
			ft_error("epoll_wait, failed");
		}

		for (int i = 0; i < nfds; ++i)
		{
			if (this->events[i].data.fd == this->_socket->getSockFd())
			{
				this->connfd = accept(this->_socket->getSockFd(), NULL, NULL);
				if (this->connfd == -1)
				{
					ft_error("accept failed");
				}

				this->ev.events = EPOLLIN | EPOLLET;
				this->ev.data.fd = this->connfd;

				if (epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, this->connfd, &ev) == -1)
				{
					ft_error("epoll_ctl (1) failed");
				}
			}
			else
			{
				handle_client();
			}
		}
	}
}

std::string	ServerExecution::getFileContent(const std::string &path)
{
	std::ifstream file(path.c_str());

	if (!file.is_open())
	{
		return (getFileContent("../www/html/errors/400.html"));
	}

	std::ostringstream oss;
	oss << file.rdbuf();
	return (oss.str());
}

std::string	ServerExecution::findContent(std::string receivedLine)
{
	std::string	path;
	size_t		path_start;
	size_t		path_end;

	path_start = receivedLine.find('/');
	if (path_start == std::string::npos)
		ft_error("path_start failed");
	path_end = (receivedLine).find(' ', path_start);

	if (path_end == std::string::npos)
		ft_error("path_end failed");
	else
		this->path = receivedLine.substr(path_start, (path_end - path_start));

	if (this->path == "/")
		return (getFileContent("../www/html/index.html"));
	else
	{
		if (this->path.compare(this->path.size()-5, 5, ".html") == 0 ) // Le this->path finis par .html
		{
			std::cout << this->path << std::endl; //TEST
			return (getFileContent("../www/html" + this->path));
		}
		else if (this->path.compare(this->path.size()-3, 3, ".css") == 0 ) // le this->path finie par css
			return (getFileContent("../www/styles" + this->path));
	}
	return (getFileContent("../www/html/errors/400.html")); //le path est une erreur
}

std::string ServerExecution::findMethod(std::string receivedLine)
{
	size_t		method_end = receivedLine.find(' ');
	
	std::cout << "\n\nTEST 1: " << receivedLine << "\n\n" << std::endl; // TEST

	if (method_end != std::string::npos)
	{
		this->method = receivedLine.substr(0, method_end);
	}
	else
	{
		ft_error("method_end failed");
	}
	return (this->method);
}

void ServerExecution::handle_client()
{
	memset(this->received_line, 0, sizeof(this->received_line));
	if (read(this->connfd, this->received_line, sizeof(this->received_line) - 1) < 0)
		ft_error("connfd read failed");
	std::string	received_line_cpy(this->received_line);
	
	this->method = findMethod(received_line_cpy);
	this->get_content = findContent(this->received_line);

	std::cout << "\n\nTEST : -" << this->get_content << "-\n\n" << std::endl; // TEST

	if (this->method == "GET")
	{
		snprintf(this->socket_buffer, sizeof(this->socket_buffer),
				 "HTTP/1.0 200 OK\r\n\r\n%s",
				 this->get_content.c_str());
	}
	else if (this->method == "POST")
	{
		snprintf(this->socket_buffer, sizeof(this->socket_buffer),
				 "HTTP/1.0 200 OK\r\n\r\n"
				 "Received POST request\n");
	}
	else if (this->method == "DELETE")
	{
		snprintf(this->socket_buffer, sizeof(this->socket_buffer),
				 "HTTP/1.0 200 OK\r\n\r\n"
				 "Received DELETE request\n");
	}
	else
	{
		snprintf(this->socket_buffer, sizeof(this->socket_buffer),
				 "HTTP/1.0 400 Bad Request\r\n\r\n%s",
				 this->get_content.c_str());
	}
	write(this->connfd, this->socket_buffer, strlen(this->socket_buffer));
	close(this->connfd);
}