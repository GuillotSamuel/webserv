/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:32:29 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/09/13 11:47:19 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

class ListeningSocket
{
	private:
	
		/*---------------------------------------------------------------*/
		/*                            ATTRIBUT                           */
		/*---------------------------------------------------------------*/
		// ServerConfiguration	&_serv;
		int					port;
		struct sockaddr_in	_server_address;
		struct protoent		*_proto;
		int					_socket_fd;
		

	public:

		/*---------------------------------------------------------------*/
		/*                    CONSTRUCTOR/DESTRUCTOR                     */
		/*---------------------------------------------------------------*/
		ListeningSocket(int port);
		~ListeningSocket();

		/*---------------------------------------------------------------*/
		/*                            METHOD                             */
		/*---------------------------------------------------------------*/
		int					getSocket_fd() const;
		void				set_nonblocking(int sockfd);
		int					getPort() const;
		/*---------------------------------------------------------------*/
		/*                         OVERLOADED                            */
		/*---------------------------------------------------------------*/
		bool				operator==(const ListeningSocket& other) const;
		bool				operator<(const ListeningSocket& other) const;
};