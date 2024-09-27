/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 22:07:42 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/09/27 12:26:17 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "webserv.hpp"

class Response
{
	private:
		/*---------------------------------------------------------------*/
		/*                   ATTRIBUT SERVER/LOCATION             		 */
		/*---------------------------------------------------------------*/
		std::string							_serverName;
		std::string                 		_alias; // vrai root vers mais ne remplace le root du serveur
		std::string							_root;
		std::string							_index;
		std::string							_cgiPath;
		std::string							_uploadsLocation;
		std::string							_filePath; // absolute path to file requested
		std::map<std::string, int>			_allowed_methods;
		std::map<int, std::string>			_errorPages;
		std::map<int, std::string>			_redirection;
		std::map<std::string, std::string>	_pathInfoCgi;
		int									_clientMaxBodySize;
		int									_autoIndex;
		int									_status;
		Client								*_client;

		/*---------------------------------------------------------------*/
		/*                   FONCTION GENERATE RESPONSE            		 */
		/*---------------------------------------------------------------*/
		std::string							ft_badRequest();
		std::string							ft_delete();
		std::string							ft_post();
		std::string							ft_get();
		std::string							ft_forbidden();
		std::string							readFileContent(std::string path);
		std::string							getMimeType();
		void								setFilePath(std::string root, std::string fileRequested);

	public:
											Response(Client *client);
											~Response();
		void								setInfo(ServerConfiguration *serv, Location *location);
		std::string							generateResponse();
		std::string							getAlias() const;
		std::string							getRoot() const;
		std::string							getIndex() const;
		std::string							getPathCgi() const;
		std::string							getUploadsLocation() const;
		std::string							getFilePath() const;
		std::string							getServerName() const;
		std::map<std::string, int>			getAllowedMethodsTab() const;
		std::map<int, std::string>			getErrorPage() const;
		std::map<int, std::string>			getRedirection() const;
		std::map<std::string, std::string>	getCgi() const;
		int									getAutoIndex() const;
		int									getClientMaxBodySize() const;
};
		std::ostream						&operator<<(std::ostream &Cout, Response const &response);