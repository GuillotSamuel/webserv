/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:32:32 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/09/05 13:40:19 by sguillot         ###   ########.fr       */
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
		int													_n_server;
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
		void												ft_post(Client client, std::string filePath, ServerConfiguration);
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
		void												split_servers(std::vector<std::string> tokens);
		void												error(std::string errorType);

	public:
		void												startingServer();
		void												serverExecution();

		/*---------------------------------------------------------------*/
		/*                    CONSTRUCTOR/DESTRUCTOR                     */
		/*---------------------------------------------------------------*/
		Server(int argc, char **argv);
		~Server();
};