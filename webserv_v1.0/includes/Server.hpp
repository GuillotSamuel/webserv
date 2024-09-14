/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:32:32 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/09/14 16:01:15 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "listeningSocket.hpp"

class ListeningSocket;
class ServerConfiguration;
class Client;

class Server
{
	private:

		/*---------------------------------------------------------------*/
		/*                            ATTRIBUT                           */
		/*---------------------------------------------------------------*/	
		struct sockaddr										_clientAdress;
		struct sockaddr_in									_address;
		struct epoll_event									_event;
		struct epoll_event									_events[MAX_EVENTS];
		int													_epoll_fd;
		int													_connexion_fd;
		int													_status_code;
		std::string											_path;
		std::string											_extensionPath;
		char												received_line[BUFFER_SIZE];
		char												socket_buffer[BUFFER_SIZE];
		std::vector<ServerConfiguration>					tab_serv;
		std::vector<ListeningSocket*>						_listSockets;
		std::map<std::string, std::string>					extpath;
		std::map<std::string, std::string>					mimePath;
		ServerConfiguration									*currentConfig;
		int													fd_config;
		bool												insideServerBlock;
		bool												insideParamBlock;
		bool												location_started;

		/*---------------------------------------------------------------*/
		/*                            METHOD                             */
		/*---------------------------------------------------------------*/

		std::map<std::string, std::string>					createExtPath();
		std::map<std::string, std::string>					createMimePath();
		void												creatAllListeningSockets();
		void												handle_client(ListeningSocket *list);
		std::string											findPath(const std::string &receivedLine);
		void												ft_get(std::string filePath);
		void												ft_post(Client client, std::string filePath);
		void												ft_delete(std::string filePath);
		void												ft_badRequest();
		std::string											readFileContent(const std::string &path);
		std::string 										getMimeType();
		void												set_nonblocking(int sockfd);
		void												saveFile(const std::string &filename, const std::string &data);
		std::string											readHead(Client *client);
		void												getServConfig(Client *client, ListeningSocket *list);
		std::string											readBody(Client *client, std::string *receivedLine);
		void												closeServer();
		void												dlFile(std::string *receivedLine, Client *client);
		void												parsing_g(int argc, char **argv);
		void												check_parsing(void);
		void												check_error_page(ServerConfiguration server_conf);
		void												check_host_page(ServerConfiguration server_conf);
		void												check_index(ServerConfiguration server_conf);
		void												check_listen(ServerConfiguration server_conf);
		void												check_location(ServerConfiguration server_conf);
		void												check_max_body(ServerConfiguration server_conf);
		void												check_root(ServerConfiguration server_conf);
		void												check_server_name(ServerConfiguration server_conf);
		void 												readConfigurationFile(const char *arg);
		void												ft_tokenizer(std::string line);
		void												error(std::string errorType);
		void												ft_param_set_tokens(std::vector<std::string> tokens);
		void												ft_invalid_line(std::vector<std::string> tokens);
		void 												ft_set_tokens(std::vector<std::string> tokens);
		void 												ft_set_server_name_param(std::vector<std::string> tokens);
		void												ft_set_client_max_body_param(std::vector<std::string> tokens);
		void												ft_set_location_param(std::vector<std::string> tokens);
		void												ft_set_listen_param(std::vector<std::string> tokens);
		void												ft_set_host_name_param(std::vector<std::string> tokens);
		void												ft_set_error_page_param(std::vector<std::string> tokens);
		void												ft_set_root_param(std::vector<std::string> tokens);
		void												ft_set_index_param(std::vector<std::string> tokens);
		void												ft_set_allowed_methods(std::vector<std::string> tokens);
		void												ft_set_cgi_bin_location(std::vector<std::string> tokens);
		void												ft_set_errors_location(std::vector<std::string> tokens);
		void												ft_set_uploads_location(std::vector<std::string> tokens);
		void												ft_location_pages(std::vector<std::string> tokens);
		void												ft_location_pages_dispatch(std::vector<std::string> current_param);

		void												creatAllSocket();

	public:
		void												startingServer();
		void												serverExecution();
		static void											log(std::string error, int type);
 		static std::ofstream								*_log;


		/*---------------------------------------------------------------*/
		/*                    CONSTRUCTOR/DESTRUCTOR                     */
		/*---------------------------------------------------------------*/
		Server(int argc, char **argv);
		~Server();
};