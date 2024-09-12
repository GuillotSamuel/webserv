/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfiguration.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <mmahfoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:41:27 by sguillot          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/09/12 13:39:26 by mmahfoud         ###   ########.fr       */
=======
/*   Updated: 2024/09/12 11:58:31 by sguillot         ###   ########.fr       */
>>>>>>> a9a92301f7197a45d09ec957f795a0973cb8c5b0
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

#define HTML_FILES "/html"
#define CSS_FILES "/styles"
#define JS_FILES "/js"
#define JSON_FILES "/data"
#define IMAGE_FILES "/images"
#define VIDEO_FILES "/videos"
#define AUDIO_FILES "/audio"
#define FONT_FILES "/fonts"
#define PDF_FILES "/docs"
#define XML_FILES "/xml"
#define ICON_FILES "/icons"
#define CSV_FILES "/data"
#define ERROR_400_PAGE "/html/errors/400.html"
#define CGI_FILES "/cgi-bin"

class ListeningSocket;

class ServerConfiguration
{
	private:
		/*---------------------------------------------------------------*/
		/*                            ATTRIBUT                           */
		/*---------------------------------------------------------------*/	
		std::map<std::string, std::string>	_pathInfoMime;
		std::map<std::string, std::string>	_pathInfoCgi;
		std::map<std::string, std::string>	_location;
		std::map<int, std::string>			errorPages;
		std::string							hostName;
		std::string							serverName;
		std::string							strPort;
		std::string							root;
		std::string							root_index;
		std::string							index;
		std::vector<int>					_port;
		int									port;
		int									clientMaxBodySize;
		std::vector<ListeningSocket*>		tab_list;

	public:

		/*---------------------------------------------------------------*/
		/*                    CONSTRUCTOR/DESTRUCTOR                     */
		/*---------------------------------------------------------------*/
		ServerConfiguration();
		// ServerConfiguration(std::string port);
		// ServerConfiguration(const ServerConfiguration &copy);
		~ServerConfiguration(void);
		void								creatMultiPort();

		/*---------------------------------------------------------------*/
		/*                         OVERLOADED                            */
		/*---------------------------------------------------------------*/
		ServerConfiguration	&operator=(const ServerConfiguration &copy);
		
		/*---------------------------------------------------------------*/
		/*                            SETTER                             */
		/*---------------------------------------------------------------*/
		void								setPort(std::string str);
		void								setHostName(std::string str);
		void								setServerName(std::string str);
		void								setErrorPage(int code, std::string str);
		void								setClientMaxBodySize(std::string str);
		void								setRoot(std::string str);
		void								setIndex(std::string str);
		void								setLocation(std::string page, std::string location);

		/*---------------------------------------------------------------*/
		/*                            GETTER                             */
		/*---------------------------------------------------------------*/
		std::string							getStrPort(void) const;
		std::string							getRoot(void) const;
		std::string							getRootIndex(void) const;
		int									getPort(void) const;
		std::vector<int>					getPortTab(void) const;
		std::string							getHostName(void) const;
		std::string							getServerName(void) const;
		std::string							getErrorPage(int code) const;
		int									getClientMaxBodySize(void) const;
		std::map<std::string, std::string>	getPathInfoCgi() const;
		std::vector<ListeningSocket*>		getTabList() const;
		std::string							getIndex() const;

		/*---------------------------------------------------------------*/
		/*                            UTILS                              */
		/*---------------------------------------------------------------*/
		void								error(std::string errorType);
};

std::ostream	&operator<<(std::ostream &Cout, ServerConfiguration const &i);
