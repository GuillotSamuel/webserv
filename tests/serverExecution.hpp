#pragma once

#include "server.hpp"
#include "listeningSocket.hpp"

class ListeningSocket;

class ServerExecution
{
	private:
		ListeningSocket		*_socket;
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
		ServerExecution(char *port);
		~ServerExecution();

		void		serverExecutionFunction();
		std::string	findContent(std::string receiveLine);
		std::string	findMethod(std::string method);
		void		handle_client();
		std::string	getFileContent(const std::string &path);
};
