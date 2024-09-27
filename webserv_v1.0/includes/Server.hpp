/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:32:32 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/09/27 12:32:35 by mmahfoud         ###   ########.fr       */
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

		void												creatAllListeningSockets();
		void												acceptConnexion(int sock);
		void												set_nonblocking(int sockfd);
		void												inConnexion(ListeningSocket *list, int connexionFD);
		void												outConnexionClient(int connexionFD);
		void												outConnexionServer(int connexionFD);
		void												handle_client(ListeningSocket *list, int current_fd);
		void												getServBlock(Client *client, ListeningSocket *list);
		void												getLocationBlock(Client *client);
		void												closeServer();

	
		void												cgiExecution(std::string filePath, Client client);
		std::string 										getMimeType(Client *client);
		void												saveFile(const std::string &filename, const std::string &data);
		std::string											readHead(Client *client);
		std::string											readBody(Client *client, std::string *receivedLine);
		void												dlFile(std::string *receivedLine, Client *client);

					/*-------------PARSING HANDLING-------------*/
		void												parsing_g(int argc, char **argv);
		void												check_parsing(void);
		void												check_error_page(ServerConfiguration server_conf);
		void												check_index(ServerConfiguration server_conf);
		void												check_listen(ServerConfiguration server_conf);
		void												check_location(ServerConfiguration server_conf);
		void												check_max_body(ServerConfiguration server_conf);
		void												check_root(ServerConfiguration server_conf);
		void												check_server_name(ServerConfiguration server_conf);
		void												check_interpreter_map(ServerConfiguration server_conf);
		void 												check_uploads(ServerConfiguration server_conf);
		void												check_path_cgi(ServerConfiguration server_conf);
		void												check_folder(const std::string &folder_path, const std::string &server_name);
		void												check_file(const std::string &folder_path, const std::string &file_path, const std::string &server_name);
		void 												check_error_code(int error_code, const std::string &server_name);
		void 												check_language(const std::string interpreter_language, const std::string &server_name);
		void 												location_check_blockName(Location location_conf, ServerConfiguration server_conf);
		void 												location_check_alias(Location location_conf, ServerConfiguration server_conf);
		void 												location_check_root(Location location_conf, ServerConfiguration server_conf);
		void 												location_check_maxBodySize(Location location_conf, ServerConfiguration server_conf);
		void 												location_check_index(Location location_conf, ServerConfiguration server_conf);
		void 												location_check_uploadsLocation(Location location_conf, ServerConfiguration server_conf);
		void 												location_check_errorPages(Location location_conf, ServerConfiguration server_conf);
		void 												location_check_cgiPath(Location location_conf, ServerConfiguration server_conf);
		void 												location_check_cgi(Location location_conf, ServerConfiguration server_conf);
		void 												readConfigurationFile(const char *arg);
		void												ft_tokenizer(std::string line);
		void												error(std::string errorType);
		void												ft_param_set_tokens(std::vector<std::string> tokens);
		void												ft_invalid_line(std::vector<std::string> tokens);
		void 												ft_set_tokens(std::vector<std::string> tokens);
		void												ft_set_auto_index(std::vector<std::string> tokens);
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
		void												ft_location_pages_dispatch(std::vector<std::string> current_param, Location &new_location);
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