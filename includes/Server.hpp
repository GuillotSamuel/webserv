#pragma once

#include "listeningSocket.hpp"

class ListeningSocket;
class ServerConfiguration;
class Client;

class Server
{
	private:
		ServerConfiguration					*_serv;
		ListeningSocket 					*_socket;
		struct sockaddr						_clientAdress;
		struct sockaddr_in					_address;
		struct epoll_event					_event;
		struct epoll_event					_events[MAX_EVENTS];
		int									_epoll_fd;
		int									_connexion_fd;
		std::string							_path;
		std::string							_extensionPath;
		char								received_line[BUFFER_SIZE];
		char								socket_buffer[BUFFER_SIZE];
		std::map<std::string, std::string>	extpath;
		std::map<std::string, std::string>	mimePath;

		std::map<std::string, std::string>	createExtPath();
		std::map<std::string, std::string>	createMimePath();
		void								handle_client();
		std::string							findPath(const std::string &receivedLine);
		void								ft_get(std::string filePath);
		void								ft_post(Client client, std::string filePath);
		void								ft_delete();
		void								ft_badRequest();
		std::string							readFileContent(const std::string &path);
		std::string 						getMimeType();
		void								set_nonblocking(int sockfd);

	public:
		void								startingServer();
		void								serverExecution();
		Server(int argc, char **argv);
		~Server();
};