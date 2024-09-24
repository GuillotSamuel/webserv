/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:32:32 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/09/24 14:20:13 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// #include "listeningSocket.hpp"
#include "webserv.hpp"

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
		std::vector<ServerConfiguration>					tab_serv;
		std::vector<ListeningSocket*>						_listSockets;
					/*--------------EPOLL HANDLING--------------*/
		struct epoll_event									_event;
		struct epoll_event									_events[MAX_EVENTS];
		int													_epoll_fd;
		std::vector<uint32_t>								_connexion_fd;

					/*-------------REQUEST HANDLING-------------*/
		int													_status_code;
		std::string											_path;
		std::string											_extensionPath;
		char												received_line[BUFFER_SIZE];
		char												socket_buffer[BUFFER_SIZE];
		std::map<std::string, std::string>					extpath;
		std::map<std::string, std::string>					mimePath;
		ServerConfiguration									*currentConfig;
		Location											*_currentLocation;
		int													fd_config;

					/*-------------PARSING HANDLING-------------*/		
		bool												insideServerBlock;
		bool												insideParamBlock;
		bool												location_started;
		std::string											_response;

					/*--------------CGI HANDLING----------------*/
		int													_is_cgi;
		std::string											_executer_cgi;
		
		/*---------------------------------------------------------------*/
		/*                            METHOD                             */
		/*---------------------------------------------------------------*/

		//ajout
		void												getLocationBlock(Client *client);
		void												acceptConnexion(int sock);
		void												inConnexion(ListeningSocket *list, int connexionFD);
		void												outConnexionClient(int connexionFD);
		void												outConnexionServer(int connexionFD);
		void												applyConfig(Client *client);

	
		void												creatAllListeningSockets();
		void												handle_client(ListeningSocket *list, int current_fd);
		void												ft_get(Client *client);
		void												ft_post(Client *client);
		void												ft_delete(Client *client);
		void												ft_badRequest();
		void												cgiExecution(std::string filePath, Client client);
		std::string											readFileContent(std::string path);
		std::string 										getMimeType(Client *client);
		void												set_nonblocking(int sockfd);
		void												saveFile(const std::string &filename, const std::string &data);
		std::string											readHead(Client *client);
		void												getServBlock(Client *client, ListeningSocket *list);
		std::string											readBody(Client *client, std::string *receivedLine);
		void												closeServer();
		void												dlFile(std::string *receivedLine, Client *client);

					/*-------------PARSING HANDLING-------------*/
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
		void    											ft_set_auto_index(std::vector<std::string> tokens);
		void 												ft_set_server_name_param(std::vector<std::string> tokens);
		void												ft_set_client_max_body_param(std::vector<std::string> tokens);
		void												ft_set_location_param(std::vector<std::string> tokens);
		void												ft_set_listen_param(std::vector<std::string> tokens);
		void												ft_set_error_page_param(std::vector<std::string> tokens);
		void												ft_set_root_param(std::vector<std::string> tokens);
		void												ft_set_index_param(std::vector<std::string> tokens);
		void												ft_set_allowed_methods(std::vector<std::string> tokens);
		void												ft_set_cgi_bin_location(std::vector<std::string> tokens);
		void												ft_set_errors_location(std::vector<std::string> tokens);
		void												ft_set_uploads_location(std::vector<std::string> tokens);
		void												ft_location_pages(std::vector<std::string> tokens);
		void												ft_location_pages_dispatch(std::vector<std::string> current_param, Location new_location);
		void												ft_checkIp(const std::string &ip_str);
		void												ft_checkPort(const std::string port_str);

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