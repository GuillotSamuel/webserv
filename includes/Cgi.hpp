/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:32:48 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/09/06 14:50:41 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include "webserv.hpp"

class Client;

class Cgi
{
	private:

		/*---------------------------------------------------------------*/
		/*                          ATTRIBUT                             */
		/*---------------------------------------------------------------*/
		std::map<std::string, std::string>	_env;
		std::map<std::string, std::string>	*_pathInfoCgi;
		char								**_myEnvp;
		char								**_myArgv;
		const char							*_path;

	public:
		/*---------------------------------------------------------------*/
		/*                    CONSTRUCTOR/DESTRUCTOR                     */
		/*---------------------------------------------------------------*/
		Cgi();
		~Cgi();

		/*---------------------------------------------------------------*/
		/*                            METHOD                             */
		/*---------------------------------------------------------------*/
		std::string							executeCgi();
		char								**conversionEnvFunc();
		char								**createArgv();
		void								setEnv(ServerConfiguration *server, Client client);
		void								setPath(const char *path);
		void								error(std::string errorType);
		void								setPathInfoCgi(std::map<std::string, std::string> *map);
};

#endif