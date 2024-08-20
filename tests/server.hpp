#pragma once

#include "listeningSocket.hpp"

class ListeningSocket;

class Server
{
	private:
		ListeningSocket 	*_socket;
		struct epoll_event	_event;
		struct epoll_event	_events[MAX_EVENTS];
		int					_epoll_fd;
		int					_connexion_fd;
		std::string			_method;
		std::string			_path;
		char				**_envp;
		char				received_line[BUFFER_SIZE];
		char				socket_buffer[BUFFER_SIZE];

		void				ServerExecution();
		void				error(std::string errorType);
		void				handle_client();
		std::string			findPath(const std::string &receivedLine);
		std::string			findMethod(const std::string &receivedLine);
		void				ft_get(std::string filePath);
		void				ft_post(std::string received_line);
		void				ft_delete();
		void				ft_badRequest(std::string filePath);
		std::string			readFileContent(const std::string &path);
		std::string 		getMimeType(const std::string &path);

	public:
		Server(char *port, char **envp);
		~Server();
};