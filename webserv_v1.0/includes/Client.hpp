/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:32:44 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/09/18 19:27:05 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	CLIENT_HPP
#define CLIENT_HPP
#include "webserv.hpp"

class Client
{
	private:
	
		/*---------------------------------------------------------------*/
		/*                            ATTRIBUT                           */
		/*---------------------------------------------------------------*/	
		std::string	_method;
		std::string	_ipAdress;
		std::string	_httpAcceptMime; // text/*, image/jpeg, image/png, image/*, */*
		std::string	_httpAcceptLanguage; // fr_CA, fr
		std::string	_httpUserAgent; // la naviguateur utilise par le client // Mozilla/5.0 (compatible; Konqueror/3; Linux)
		std::string	_httpReferer; //Une adresse absolue ou partielle de la page web à partir de laquelle la requête vers la page courante a été émise.
		std::string	_contentType;
		std::string	_contentLength;
		std::string	_boundary;
		std::string	_host;
		std::string	_fullPath; // path with the '/' at start
		std::string	_path; // path without the '/' at start
		std::string	_filePath; // absoluth path of the file requested
		int			_currentFd; // fd of the current socketclient
		

	public:

		/*---------------------------------------------------------------*/
		/*                    CONSTRUCTOR/DESTRUCTOR                     */
		/*---------------------------------------------------------------*/
		Client();
		~Client();

		/*---------------------------------------------------------------*/
		/*                            METHOD                             */
		/*---------------------------------------------------------------*/
		void		setInfo(std::string info);
		void		error(std::string errorType);

		/*---------------------------------------------------------------*/
		/*                            GETTER                             */
		/*---------------------------------------------------------------*/
		std::string getMethod() const;
		std::string getIpAdress() const;
		std::string getAcceptMime() const;
		std::string getAcceptLanguage() const;
		std::string getUserAgent() const;
		std::string getReferer() const;
		std::string getContentType() const;
		std::string getContentLength() const;
		std::string getBoundary() const;
		std::string getPath() const;
		std::string	getHost() const;
		std::string	getFullPath() const;
		std::string	getFilePath() const;
		int			getCurrentFd() const;

		/*---------------------------------------------------------------*/
		/*                            SETTER                             */
		/*---------------------------------------------------------------*/
		void		setMethod(std::string method);
		void		setIpAddress(std::string ipAdress);
		void		setAcceptMime(std::string httpAcceptMime);
		void		setAcceptLanguage(std::string httpAcceptLanguage);
		void		setUserAgent(std::string userAgent);
		void		setReferer(std::string referer);
		void		setContentType(std::string contentType);
		void		setContentLength(std::string contentLength);
		void		setBoundary(std::string boundary);
		void		setHost(std::string host);
		void		setFullPath(std::string str);
		void		setCurrentFd(int currentFd);
		void		setFilePath(std::string filePath);

};

std::ostream	&operator<<(std::ostream &Cout, Client const &i);

#endif