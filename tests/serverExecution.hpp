#pragma once

#include "server.hpp"
#include "socket.hpp"

class ServerExecution
{
	private:
		ListeningSocket		socket;
		int					epoll_fd;
		int					connfd;
		struct epoll_event	ev;
		struct epoll_event	events[MAX_EVENTS];
		char				received_line[BUFFER_SIZE];
		char				socket_buffer[BUFFER_SIZE];
		std::string			method;
		std::string			path;
		std::string			get_content;

	public:
		ServerExecution(ListeningSocket socket);
		~ServerExecution();

		void	serverExecutionFunction();
		void	handle_client(int connfd);


}

ServerExecution::ServerExecution(ListeningSocket socket)
{
	this->epoll_fd = epoll_create(1);
	if (this->epoll_fd == -1)
		ft_error("epoll_create1 failed", -1);

	this->ev.events = EPOLLIN;
	this->ev.data.fd = socket.getSockFd();

	if (epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, socket.getSockFd(), &this->ev) == -1)
	{
		ft_error("epoll_ctl failed", -1);
	}


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
			ft_error("epoll_wait, failed", -1);
			
		for (int i = 0; i < nfds; ++i)
		{
			if (this->events[i].data.fd == this->sockfd)
			{
				this->connfd = accept(this->sockfd, NULL, NULL);
				if (this->connfd == -1)
					ft_error("accept failed", -1);

				this->ev.events = EPOLLIN | EPOLLET;
				this->ev.data.fd = this->connfd;

				if (epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, this->connfd, &ev) == -1)
					ft_error("epoll_ctl (1) failed", this->connfd);
			}
			else
			{std::string

void ServerExecution::handle_client(int connfd)
{
	memset(this->received_line, 0, sizeof(this->received_line));

	if (read(this->connfd, this->received_line, sizeof(this->received_line) - 1) < 0)
		ft_error("connfd read failed", this->connfd);

	std::string	received_line_cpy(this->received_line);
	size_t		method_end = received_line_cpy.find(' ');
	
	std::cout << "\n\nTEST 1: " << received_line_cpy << "\n\n" << std::endl; // TEST

	if (method_end != std::string::npos)
	{
		this->method = received_line_cpy.substr(0, method_end);
	}
	else
	{
		ft_error("method_end failed", this->connfd);
	}

	/* ----- PATH PARSE ----- */

	this->get_content = findContent(this->received_line);
	
	/* ----- METHOD ----- */

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
