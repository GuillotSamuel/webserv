/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfiguration.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:41:27 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/17 23:03:42 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

#define HTML_FILES "/html"
#define CSS_FILES "/styles"
#define JS_FILES "/scripts"
#define JSON_FILES "/data"
#define IMAGE_FILES "/images"
#define	ICONS_FILES "/images/icons"
#define VIDEO_FILES "/videos"
#define AUDIO_FILES "/audio"
#define FONT_FILES "/fonts"
#define PDF_FILES "/docs"
#define XML_FILES "/xml"
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

		/* --- General informations from configuration file --- */
		std::string							_serverName; // ok
		std::string							_hostName; // ok
		std::vector<int>					_port; // ok
		std::string							_root;  // ok //Path_du_site
		std::string							_index; // ok
		int									_clientMaxBodySize; // ok
		std::map<std::string, std::string>	_pathInfoCgi; // ok
		std::map<std::string, int>			_allowed_methods; // ok
		std::map<int, std::string>			_errorPages; // ok
		int									_autoIndex; // ok

		/* --- Locations from configuration file --- */
		std::string							_errorPagesLocation; // ok
		std::string							_cgiBin_location; // ok
		std::string							_uploadsLocation; // ok
		std::map<std::string, std::string>	_interpreter_map; // ADDING IN <<
		std::map<std::string, t_location>	_locations_map; // ok // adding _

		/* --- Div informations --- */
		std::string							root_index; // ok
		std::string							imHere; // ok // ou est ce que je suis

		/* --- WE DON'T KNOW YET (because Marianne is trop forte) --- */
		std::map<std::string, std::string>	_pathInfoMime; // TO REMOVE ???
		std::string							strPort; // a check

	public:
		/*---------------------------------------------------------------*/
		/*                    CONSTRUCTOR/DESTRUCTOR                     */
		/*---------------------------------------------------------------*/
		ServerConfiguration();
		~ServerConfiguration(void);

		/*---------------------------------------------------------------*/
		/*                         OVERLOADED                            */
		/*---------------------------------------------------------------*/
		ServerConfiguration &operator=(const ServerConfiguration &copy);
		
		/*---------------------------------------------------------------*/
		/*                            SETTER                             */
		/*---------------------------------------------------------------*/
		void								setPort(std::string str);
		void								setHostName(std::string str);
		void								setServerName(std::string str);
		void								setErrorPages(int code, std::string str);
		void								setClientMaxBodySize(std::string str);
		void								setRoot(std::string str);
		void								setIndex(std::string str);
		void								setInterpreterMap(std::string page, std::string location);
		void								setAllowedMethods(std::string method, int code);
		void								setUploadsLocation(std::string str);
		void								setErrorPagesLocation(std::string str);
		void								setPathInfoCgi(std::string extension, std::string location);
		void								setCgiBinLocation(std::string str);
		void								setRootIndex();
		void								setAutoIndex(std::string str);
		void								setLocationMap(std::string location_key, t_location new_location);
		
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
		std::map<int, std::string>			getErrorPages(void) const;
		int									getClientMaxBodySize(void) const;
		std::map<std::string, std::string>	getPathInfoCgi(void) const;
		std::string							getIndex(void) const;
		std::string							getUploadLocation(void) const;
		std::string							getErrorPageLocation(void) const;
		std::string							getCgiLocation(void) const;
		std::map<std::string, std::string>	getInterpreterMap(void) const;
		std::map<std::string, std::string>	getInfoMime(void) const;
		std::map<std::string, t_location>	getTabLocation(void) const;
		std::map<std::string, int>			getAllowedMethods(void)const;
		std::string							getimHere() const;
		std::string							getAutoIndex() const;
		std::map<std::string, t_location>	getLocationMap() const;
		
		/*---------------------------------------------------------------*/
		/*                            UTILS                              */
		/*---------------------------------------------------------------*/
		void								error(std::string errorType);
};

std::ostream	&operator<<(std::ostream &Cout, ServerConfiguration const &i);
