/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:32:32 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/09/09 12:43:56 by sguillot         ###   ########.fr       */
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
		std::string											_path;
		std::string											_extensionPath;
		char												received_line[BUFFER_SIZE];
		char												socket_buffer[BUFFER_SIZE];
 		std::ofstream										*_log;
		ListeningSocket										**tab_list;
		std::vector<ServerConfiguration>					tab_serv;
		std::map<ListeningSocket *, ServerConfiguration *>	_config;
		std::map<std::string, std::string>					extpath;
		std::map<std::string, std::string>					mimePath;
		ServerConfiguration									*currentConfig;
		int													fd_config;
		bool												insideServerBlock;
		bool												insideParamBlock;

		/*---------------------------------------------------------------*/
		/*                            METHOD                             */
		/*---------------------------------------------------------------*/
		std::map<std::string, std::string>					createExtPath();
		std::map<std::string, std::string>					createMimePath();
		void												handle_client(ServerConfiguration serv);
		std::string											findPath(const std::string &receivedLine);
		void												ft_get(std::string filePath);
		void												ft_post(Client client, std::string filePath, ServerConfiguration *serv);
		void												ft_delete();
		void												ft_badRequest();
		std::string											readFileContent(const std::string &path);
		std::string 										getMimeType();
		void												set_nonblocking(int sockfd);
		void												saveFile(const std::string &filename, const std::string &data);
		std::string											readRequest(Client *client);
		void												creatMultiListenPort();
		void												log(std::string error, int type);
		void												closeServer();
		void												dlFile(std::string receivedLine, Client *client);
		void												parsing_g(int argc, char **argv);
		void 												readConfigurationFile(const char *arg);
		void												ft_tokenizer(std::string line);
		void												error(std::string errorType);
		void												ft_param_set_tokens(std::vector<std::string> tokens);
		void												ft_invalid_line(std::vector<std::string> tokens);
		void												ft_start_set_tokens(std::vector<std::string> tokens);
		void 												ft_set_tokens(std::vector<std::string> tokens);
		void 												ft_set_server_name_param(std::vector<std::string> tokens);
		void												ft_set_client_max_body_param(std::vector<std::string> tokens);
		void												ft_set_location_param(std::vector<std::string> tokens);
		void												ft_set_listen_param(std::vector<std::string> tokens);
		void												ft_set_host_name_param(std::vector<std::string> tokens);
		void												ft_set_error_page_param(std::vector<std::string> tokens);

	public:
		void												startingServer();
		void												serverExecution();

		/*---------------------------------------------------------------*/
		/*                    CONSTRUCTOR/DESTRUCTOR                     */
		/*---------------------------------------------------------------*/
		Server(int argc, char **argv);
		~Server();
};