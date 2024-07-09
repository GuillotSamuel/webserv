/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverConfiguration.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:41:27 by sguillot          #+#    #+#             */
/*   Updated: 2024/07/09 14:39:37 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIGURATION_HPP
# define SERVERCONFIGURATION_HPP

#include "webserv.hpp"

class ServerConfiguration
{
    private:
        int        					port;
        std::string					hostName;
        std::string					serverName;
		std::map<int, std::string>	errorPages;
		int							clientMaxBodySize;
        
    public:
        ServerConfiguration(void);
        ServerConfiguration(const ServerConfiguration &copy);
        ~ServerConfiguration(void);
        ServerConfiguration &operator=(const ServerConfiguration &copy);

		void		setPort(std::string str);
		void		setHostName(std::string str);
        void		setServerName(std::string str);
		void		setErrorPage(int code, std::string str);
		void		setClientMaxBodySize(std::string str);

		int			getPort() const;
		std::string	getHostName() const;
		std::string	getServerName() const;
		std::string	getErrorPage(int code) const;
		int			getClientMaxBodySize() const;
		
};

#endif