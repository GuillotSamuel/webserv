#pragma once

#include "listeningSocket.hpp"

class ListeningSocket;
class ServerConfiguration;
class Client;

class Server
{
	private:
		std::map<ListeningSocket, ServerConfiguration> _config;
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
 		std::ofstream						*_log;
		ListeningSocket						**tab_list;
		ServerConfiguration					**tab_serv;

		std::map<std::string, std::string>	createExtPath();
		std::map<std::string, std::string>	createMimePath();
		void								handle_client(ServerConfiguration serv);
		std::string							findPath(const std::string &receivedLine);
		void								ft_get(std::string filePath);
		void								ft_post(Client client, std::string filePath, ServerConfiguration);
		void								ft_delete();
		void								ft_badRequest();
		std::string							readFileContent(const std::string &path);
		std::string 						getMimeType();
		void								set_nonblocking(int sockfd);
		void								saveFile(const std::string &filename, const std::string &data);
		std::string							readRequest(Client *client);
		void								creatMultiListenPort(ServerConfiguration *serv[], int size);
		void								log(std::string error, int type);
	public:
		void								startingServer();
		void								serverExecution();
		Server(ServerConfiguration *serv[], int size);
		~Server();
};