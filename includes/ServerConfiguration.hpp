/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfiguration.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:41:27 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/01 20:12:37 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

#define HTML_FILES "www/html"
#define CSS_FILES "www/styles"
#define JS_FILES "www/js"
#define JSON_FILES "www/data"
#define IMAGE_FILES "www/images"
#define VIDEO_FILES "www/videos"
#define AUDIO_FILES "www/audio"
#define FONT_FILES "www/fonts"
#define PDF_FILES "www/docs"
#define XML_FILES "www/xml"
#define ICON_FILES "www/icons"
#define CSV_FILES "www/data"
#define ERROR_400_PAGE "www/html/errors/400.html"
#define CGI_FILES "/cgi-bin"

class ServerConfiguration
{
    private:
        int        							port;
		int									fd_config;
        std::string							hostName;
        std::string							serverName;
		std::map<int, std::string>			errorPages;
		int									clientMaxBodySize;
		std::map<std::string, std::string>	_pathInfoCgi;
		std::map<std::string, std::string>	_pathInfoMime;
    public:
        std::ofstream						*_log;
        ServerConfiguration(int argc, char **argv);
        ServerConfiguration(const ServerConfiguration &copy);
        ~ServerConfiguration(void);
        ServerConfiguration &operator=(const ServerConfiguration &copy);

		void								setPort(std::string str);
		void								setHostName(std::string str);
        void								setServerName(std::string str);
		void								setErrorPage(int code, std::string str);
		void								setClientMaxBodySize(std::string str);
		
		int									getPort(void) const;
		std::string							getHostName(void) const;
		std::string							getServerName(void) const;
		std::string							getErrorPage(int code) const;
		int									getClientMaxBodySize(void) const;
		std::map<std::string, std::string>	getPathInfoCgi() const;
		void								log(std::string error, int type);

		void								parsing_g(int argc, char **argv);
		void								error(std::string errorType);
		void								readConfigurationFile(const char *str);
		void								ft_tokenizer(std::string str);
};

std::ostream	&operator<<(std::ostream &Cout, ServerConfiguration const &i);
