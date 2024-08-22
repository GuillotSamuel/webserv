#pragma once

#include "listeningSocket.hpp"

class ListeningSocket;
class ServerConfiguration;

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
		void				ft_badRequest();
		std::string			execute_cgi_script();
		std::string			readFileContent(const std::string &path);
		std::string 		getMimeType(const std::string &path);

		void				handle_plain_text(const std::string &body);
		void				handle_multipart_data(const std::string &headers, const std::string &body);
		void				handle_url_encoded(const std::string &body);

	public:
		Server(ServerConfiguration inf);
		~Server();
};