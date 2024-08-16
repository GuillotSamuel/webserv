#pragma once

#include "listeningSocket.hpp"
#include "FormData.hpp"

class ListeningSocket;
class FormData;

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
		char				received_line[BUFFER_SIZE];
		char				socket_buffer[BUFFER_SIZE];
		std::list<FormData>	my_list;

		void				ServerExecution();
		std::string			findContent(const std::string &receivedLine);
		std::string			findMethod(const std::string &receivedLine);
		void				handle_client();
		std::string			getFileContent(const std::string &path);
		void				setFormData(std::string receivedLine);
		void				error(std::string errorType);
		void				ft_get(std::string get_content);
		void				ft_post(std::string received_line);
		void				ft_delete();
		void				ft_badRequest(std::string get_content);
	public:
		Server(char *port);
		~Server();
		std::list<FormData>				getList(); //TEST
};
