#pragma once

#include "listeningSocket.hpp"

class ListeningSocket;
class ServerConfiguration;
class Client;

class Server
{
	private:
		struct sockaddr		_clientAdress;
		struct sockaddr_in	_address;
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
		ServerConfiguration	_serv;

		void				ServerExecution();
		void				error(std::string errorType);
		void				handle_client();
		std::string			findPath(const std::string &receivedLine);
		std::string			findMethod(const std::string &receivedLine);
		void				ft_get(std::string filePath);
		void				ft_post(Client client, std::string filePath);
		void				ft_delete();
		void				ft_badRequest();
		std::string			readFileContent(const std::string &path);
		std::string 		getMimeType(const std::string &path);
		void				set_nonblocking(int sockfd);
	public:
		Server(ServerConfiguration inf);
		~Server();
};