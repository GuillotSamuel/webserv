/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:27:50 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/09/18 20:32:16 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

std::ofstream* Server::_log = NULL;

/*----------------------------------------------------------------------------*/
/*                               CONSTRUCTOR                                  */
/*----------------------------------------------------------------------------*/

Server::Server(int argc, char **argv)
{
	this->_response = "";
	this->_is_cgi = 0;
	_log = new std::ofstream("logfile.log", std::ios::out);
	if (!_log->is_open())
	{		
    	std::cerr << "Failed to open log file" << std::endl;
	}
	this->_status_code = 0;
	parsing_g(argc, argv);
	creatAllListeningSockets();
	log("Starting Server.", 3);
	this->_epoll_fd = -1;
	this->extpath = createExtPath();
	this->mimePath = createMimePath();
	memset(&_address, 0, sizeof(struct sockaddr_in));
	memset(&_clientAdress, 0, sizeof(struct sockaddr));
	memset(&_event, 0, sizeof(struct epoll_event));
 	memset(received_line, 0, BUFFER_SIZE);
	memset(socket_buffer, 0, BUFFER_SIZE);
}

void	Server::creatAllListeningSockets()
{
	std::vector<ServerConfiguration>::iterator it = this->tab_serv.begin();
	for (; it < this->tab_serv.end(); it++)
	{
		int boul = 0;
		it->setRootIndex();
		std::vector<int> port = it->getPortTab();
		std::vector<int>::iterator itTabPort = port.begin();
		for (; itTabPort < port.end(); itTabPort++)
		{
			std::vector<ListeningSocket*>::iterator itList = _listSockets.begin();
			for (int i = 0; itList < _listSockets.end(); itList++, i++)
			{
				if ((*itList)->getPort() == *itTabPort)
				{
					boul = 1;
					break;
				}
			}
			if (boul != 1)
			{
				_listSockets.push_back(new ListeningSocket(*itTabPort));
			}
		}
	}
}

/*----------------------------------------------------------------------------*/
/*                              METHOD/SERVER                                 */
/*----------------------------------------------------------------------------*/

/*
Creation of our instance of epoll and add all of our listening socket to the
instance so epoll_wait can watch all of them.
*/
void	Server::startingServer()
{
	if ((this->_epoll_fd = epoll_create(1)) == -1)
		log("Epoll instance creation failed.", 2);
	
	log("Epoll instance successfully created.", 1);
	std::vector<ListeningSocket*>::iterator itTab = this->_listSockets.begin();
	for(; itTab != this->_listSockets.end(); itTab++)
	{
		this->_event.events = EPOLLIN;
		this->_event.data.fd = (*itTab)->getSocket_fd();

		if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, (*itTab)->getSocket_fd(), &this->_event) == -1)
		{
			log("Server failed to add the socket file descriptor to his instance of Epoll.", 2);
			exit(EXIT_FAILURE);
		}
	}
	log("Epoll_ctl successfully add the socket file descriptor to Epoll instance.", 1);
	log("The server construction is now complete. The server is ready to accept incoming connections.", 1);
}

/*
-Waiting for new connexions
-create disposable socket with accept 
-handle request
*/
void Server::serverExecution()
{
	while (true)
	{
		int nfds = epoll_wait(this->_epoll_fd, this->_events, MAX_EVENTS, -1);
		if (nfds == -1)
			log("The call to epoll_wait failed.", 2);
		if (g_signal == SIGNAL)
			closeServer();
		ListeningSocket *list = NULL;
		for (int i = 0; i < nfds; i++)
		{
			int	sock = 0;
			std::vector<ListeningSocket*>::iterator it = _listSockets.begin();
			for (; it < _listSockets.end(); it++)
			{
				if (this->_events[i].data.fd == (*it)->getSocket_fd())
				{
					sock = (*it)->getSocket_fd();
					list = (*it);
					break;
				}
			}
			if (sock == 0)	
			{
				std::vector<uint32_t>::iterator confd = this->_connexion_fd.begin();
				for(; confd != this->_connexion_fd.end(); confd++)
				{
					if (*confd == (uint32_t)this->_events[i].data.fd)
					{
						break;
					}
				}
				if (this->_events[i].events & EPOLLIN)
				{
					handle_client(list, *confd);
					this->_event.events = EPOLLOUT | EPOLLRDHUP | EPOLLHUP;
					this->_event.data.fd = *confd;
					if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_MOD, *confd, &this->_event) == -1)
					{
						log("Epoll_ctl failed.", 2);
					}	
				}
				else if (this->_events[i].events & (EPOLLRDHUP | EPOLLHUP))
				{
					if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_DEL, *confd, &this->_event) == -1)
					{
						log("Epoll_ctl failed.", 2);
					}
					close(*confd);
					list = NULL;
					
					log("Client has been successfuly closed.", 1);
				}
				else if (this->_events[i].events & EPOLLOUT)
				{
					send(*confd, this->_response.c_str(), this->_response.size(), 0);
					if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_DEL, *confd, &this->_event) == -1)
					{
						log("Epoll_ctl failed.", 2);
					}
					close(*confd);
					list = NULL;
				}
				else
					log("Inexpected event has been detected.", 2);
			}
			else
			{
				if (this->_events[i].data.fd == sock)
				{
					socklen_t client_addrlen = sizeof(this->_clientAdress);
					this->_connexion_fd.push_back(accept(sock, (struct sockaddr *)&this->_clientAdress, &client_addrlen));
					if ((int)(this->_connexion_fd.back()) == -1)
					{
						log("The call to accept function failed for unknown reason.", 2);
					}
					log("[NEW REQUEST]", 3);
					log("Server did accept the connection.", 1);

					set_nonblocking(this->_connexion_fd.back());

					this->_event.events = EPOLLIN;
					this->_event.data.fd = this->_connexion_fd.back();
					if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, this->_connexion_fd.back(), &this->_event) == -1)
					{
						log("Epoll_ctl failed.", 2);
					}
				}
			}
		}
	}
}


/*
-Parsing request, download file if needed 
and chose what method to use
*/
void Server::handle_client(ListeningSocket *list, int current_fd)
{
	Client *client = new Client();

	char client_ip[INET_ADDRSTRLEN];
	
	memset(client_ip, 0, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &this->_address.sin_addr, client_ip, INET_ADDRSTRLEN);
	std::string ipAdress(client_ip);
	client->setIpAddress(ipAdress);
	client->setCurrentFd(current_fd);
	std::string receivedLine = readHead(client);
	if (receivedLine == "")
	{
		delete client;
		log("Server could not read anything from client. The client will be remove.", 2);
		return ;
	}
	getServConfig(client, list);
	if (this->currentConfig == NULL)
		return ;
	client->setFilePath(findPath(client));
	if (client->getMethod() == "GET")
		ft_get(client);
	else if (client->getMethod() == "POST")
		ft_post(client);
	else if (client->getMethod() == "DELETE")
		ft_delete(client);
	else
		ft_badRequest();
	delete client;
	log("End of the request.", 1);
}

std::string	Server::readHead(Client *client)
{
	int n = recv(client->getCurrentFd(), this->received_line, 4096, 0);
	if (n < 0)
	{
		log("The call recv failed.", 2);
		return ("");
	}
	if (n == 0)
	{
		log("The connexion has been interrupted.", 3);
		return ("");
	}
	std::string receivedLine(this->received_line, 4096);

	client->setInfo(receivedLine);
	return (receivedLine);
}


void	Server::getServConfig(Client *client, ListeningSocket *list)
{
	this->currentConfig = NULL;
	std::vector<ServerConfiguration>::iterator it = this->tab_serv.begin();
	for (; it < this->tab_serv.end(); it++)
	{
		if (it->getServerName() == client->getPath())
		{
			this->currentConfig = &(*it);
			return ;
		}
	}
	
	it = this->tab_serv.begin();
	for (; it < this->tab_serv.end(); it++)
	{
		if (it->getHostName() == client->getHost())
		{
			this->currentConfig = &(*it);
			return ;
		}
	}

	it = this->tab_serv.begin();
	for (; it < this->tab_serv.end(); it++)
	{
		std::vector<int> tab = it->getPortTab();
		std::vector<int>::iterator itPort = tab.begin();
		for (; itPort < tab.end(); itPort++)
		{
			if (*itPort == list->getPort())
			{
				this->currentConfig = &(*it);
				return ;
			}
		}
	}
	this->currentConfig = NULL;
}

std::string	Server::readBody(Client *client, std::string *receivedLine)
{
	if (client->getContentLength() != "")
	{
		int len = atoi(client->getContentLength().c_str());

        char *buffer = new char[len + 1];
        memset(buffer, 0, len + 1);
        int total_read = 0;
		while (total_read < len)
        {
            int read = recv(client->getCurrentFd(), buffer + total_read, len - total_read, 0);
            if (read < 0)
            {
                log("Recv failed.", 2);
                break ;
            }
            if (read == 0)
            {
                log("The connection has been interrupted.", 2);
                break ;
            }
            total_read += read;
        }

		(*receivedLine).append(buffer, total_read);
    	delete[] buffer;
		dlFile(receivedLine, client);
	}

	std::ofstream file("request.txt");
	if (file.is_open()) {
		file << *receivedLine;
		file.close();
		log("File is created.", 1);	} 
	else
		log("Unable to open file.", 2);
	return (*receivedLine);
}
void	Server::cgiExecution(std::string filePath, Client client)
{
	Cgi *cgi = new Cgi();
	cgi->setExecuter(this->_executer_cgi);
	cgi->setPath(filePath.c_str());
	cgi->setEnv(this->currentConfig, client); 
	std::string content = cgi->executeCgi();

	std::string mimeType = getMimeType(&client);

	std::string response = "HTTP/1.1 200 OK\r\n";
	response += "Content-Type: text/html\r\n";
	std::ostringstream oss;
	oss << content.size();
	response += "Content-Length: " + oss.str() + "\r\n";
	response += "Connection: close\r\n";
	response += "Server: " + this->currentConfig->getServerName() + "\r\n\r\n";
	response += content;

	this->_response = response;
	delete cgi;
}

/*response to a GET request*/
void Server::ft_get(Client *client) // a revoir
{
	log("Server's receive a GET request.", 1);
	
	if (this->_is_cgi == 1)
	{
		cgiExecution(client->getFilePath(), *client);
	}
	else
	{
		std::string content = readFileContent(client->getFilePath());
		std::string response = "";
		std::stringstream ss;
		ss << this->_status_code;
		std::string _code = ss.str();
		if (this->_status_code != 0)
		{
			log("The file requested \"" + client->getFilePath() + "\" was found empty.", 1);
			content = readFileContent(client->getFilePath());

			response = "HTTP/1.1 " + _code + " Bad Request\r\n";
			response += "Content-Type: text/html\r\n";
			std::ostringstream oss;
			oss << content.size();
			response += "Content-Length: " + oss.str() + "\r\n";
			response += "Connection: close\r\n";
			response += "Server: webserv/1.0\r\n\r\n";
			response += content;
		}
		else
		{
			log("The file requested \"" + client->getFilePath() + "\" was found.", 1);
			std::string mimeType = getMimeType(client);

			response = "HTTP/1.1 200 OK\r\n";
			response += "Content-Type: " + mimeType + "\r\n";
			std::ostringstream oss;
			oss << content.size();
			response += "Content-Length: " + oss.str() + "\r\n";
			response += "Connection: close\r\n";
			response += "Server: " + this->currentConfig->getServerName() + "\r\n\r\n";
			response += content;
		}
		this->_response = response;
	}
}

/*response to a POST request*/
void Server::ft_post(Client *client) // a revoir surtout au niveau de la requete
{
	log("Server's receive a POST request.", 1);
	if (this->_is_cgi == 1)
	{
		cgiExecution(client->getFilePath(), *client);
	}
	else
	{
		std::string content = readFileContent(client->getFilePath());
		std::string response = "";
		std::stringstream ss;
		ss << this->_status_code;
		std::string _code = ss.str();
		if (this->_status_code != 0)
		{
			log("The file requested \"" + client->getFilePath() + "\" was found empty.", 1);
			content = readFileContent(client->getFilePath());

			response = "HTTP/1.1 " + _code + " Bad Request\r\n";
			response += "Content-Type: text/html\r\n";
			std::ostringstream oss;
			oss << content.size();
			response += "Content-Length: " + oss.str() + "\r\n";
			response += "Connection: close\r\n";
			response += "Server: webserv/1.0\r\n\r\n";
			response += content;
		}
		else
		{
			log("The file requested \"" + client->getFilePath() + "\" was found.", 1);
			std::string mimeType = getMimeType(client);

			response = "HTTP/1.1 200 OK\r\n";
			response += "Content-Type: " + mimeType + "\r\n";
			std::ostringstream oss;
			oss << content.size();
			response += "Content-Length: " + oss.str() + "\r\n";
			response += "Connection: close\r\n";
			response += "Server: " + this->currentConfig->getServerName() + "\r\n\r\n";
			response += content;
		}
		this->_response = response;
	}
}

/*response to a DELETE request*/
void Server::ft_delete(Client *client) // a revoir
{
	log("Server's receive a DELETE request.", 1);

	if (access(client->getFilePath().c_str(), F_OK) != 0)
	{
		std::string response = "HTTP/1.1 404 Not Found\r\n";
		response += "Content-Type: text/html\r\n";
		std::string content = "File not found";
		std::ostringstream oss;
		oss << content.size();
		response += "Content-Length: " + oss.str() + "\r\n";
		response += "Connection: close\r\n";
		response += "Server: " + this->currentConfig->getServerName() + "\r\n\r\n";
		response += "File not found";

		this->_response = response;
		return;
	}

	if (unlink(client->getFilePath().c_str()) == 0)
	{
		std::string response = "HTTP/1.1 204 No Content\r\n";
		response += "Connection: close\r\n";
		response += "Server: " + this->currentConfig->getServerName() + "\r\n\r\n";

		this->_response = response;
	} 
	else
		log("Server failed to respond at the DELETE request.", 2);
}

/*response to a bad request*/
void Server::ft_badRequest()
{
	std::string content = readFileContent("");

	std::string response = "HTTP/1.1 400 Bad Request\r\n";
	response += "Content-Type: text/html\r\n";
	std::ostringstream oss;
    oss << content.size();
    response += "Content-Length: " + oss.str() + "\r\n";
	response += "Connection: close\r\n";
	response += "Server: " + this->currentConfig->getServerName() + "\r\n\r\n";
	response += content;

	this->_response = response;
}

//Closing The server using key CTRL /C.
void	Server::closeServer()
{
	log("Shutting down the server properly.", 1);
	_log->close();
	delete _log;
	std::vector<ListeningSocket*>::iterator itList = this->_listSockets.begin();
	for (; itList < this->_listSockets.end(); itList++)
	{
		delete (*itList);
	}
	this->_listSockets.clear();
	this->tab_serv.clear();
	std::vector<ListeningSocket*>().swap(_listSockets);
	std::vector<ServerConfiguration>().swap(tab_serv);
	this->mimePath.clear();
	this->extpath.clear();
	this->_connexion_fd.clear();
	close(this->_epoll_fd);
	exit(EXIT_SUCCESS);
}

/*----------------------------------------------------------------------------*/
/*                               DESTRUCTOR                                   */
/*----------------------------------------------------------------------------*/

Server::~Server()
{
}
/*----------------------------------------------------------------------------*/
/*                                  UTILS                                     */
/*----------------------------------------------------------------------------*/

void	Server::dlFile(std::string *receivedLine, Client *client)
{
	std::string file_name;
	std::string body_content;
	size_t filename = (*receivedLine).find("filename=\"");
	if (filename != std::string::npos)
	{
		filename += 10;
		size_t endFilename  = (*receivedLine).find("\"", filename);
		if (endFilename != std::string::npos)
		{
			file_name = (*receivedLine).substr(filename, (endFilename - filename));
		
			size_t body = (*receivedLine).find("\r\n\r\n", endFilename);
			if (body != std::string::npos)
			{
				body += 4;
				size_t endbody = (*receivedLine).find(client->getBoundary(), body);
				if (endbody != std::string::npos)
				{
					body_content = (*receivedLine).substr(body, (endbody - body));
					(*receivedLine).erase(body, (endbody - body));
					saveFile(this->currentConfig->getUploadLocation() + file_name, body_content);
				}
				else
					log("The request body cannot be found.", 2);
			}
			else
				log("The request body cannot be found.", 2);

		}
		else
			log("Filename cannot be found.", 2);
	}
	else
		log("Filename cannot be found.", 2);
}

std::string	Server::readFileContent(std::string path)
{
	std::ifstream file(path.c_str());

	if (!file.is_open())
	{
		return (readFileContent(findErrorPage(404)));
	}

	std::ostringstream oss;
	oss << file.rdbuf();
	return (oss.str());
}

void	Server::saveFile(const std::string &filename, const std::string &data) // POST UPLOAD
{
    std::ofstream file(filename.c_str(), std::ios::binary);
    if (file.is_open()) 
	{
        file.write(data.c_str(), data.size());
        file.close();
		log("File " + filename + " saved successfully.", 1);
    }
	else 
	{
		log("Failed to open file " + filename + ".", 2);
    }
}

std::string	Server::getMimeType(Client *client)
{
	if (client->getFullPath() == ("/" + this->currentConfig->getServerName())
		|| client->getFullPath() == "/" )
		return ("text/html");
	
	if (this->mimePath.find(this->_extensionPath) != this->mimePath.end())
		return (this->mimePath[this->_extensionPath]);

	log("Extension of the file cannot be found.", 2);
	return ("application/octet-stream");
}


std::string Server::findPath(Client *client)
{	
	std::string receivedLine;
	/*---------------------------------------------------------------*/
	/*                  IS IT SPECIFIC LOCATION?                     */
	/*---------------------------------------------------------------*/
	if (this->currentConfig->getUploadLocation() == client->getFullPath())
	{
		//c'est pas un cgi et il faut dl
		//ici on telecharge
		//on stock
		//une redirection temporaire 304 vers une page success
		//si elle existe pas, peut etre en prendre une par default
		readBody(client, &receivedLine);
	}
	// std::map<std::string, t_location> obj = this->currentConfig->getTabLocation();
	// if (obj.find(client->getPath()) != obj.end())
	// {
	// 	t_location loc = obj[client->getPath()];

	// 	client->setFullPath(loc.alias);
	// 	// if (client->getMethod() != )
	// }
	
	/*---------------------------------------------------------------*/
	/*                   IS THIS AN ALLOWED METHOD ?                 */
	/*---------------------------------------------------------------*/
	std::map<std::string, int> method = this->currentConfig->getAllowedMethods();
	if (method.find(client->getMethod()) == method.end())
		return (findErrorPage(403));
	else
	{
		if (method[client->getMethod()] == 0)
			return (findErrorPage(403));
	}
	/*---------------------------------------------------------------*/
	/*                         IS IT INDEX ?                         */
	/*---------------------------------------------------------------*/
	if (client->getFullPath() == ("/" + this->currentConfig->getServerName())
	|| client->getFullPath() == "/")
	{
		this->_status_code = 0;
		return (this->currentConfig->getRootIndex());
	}
	/*---------------------------------------------------------------*/
	/*             		  DOES IT HAVE EXTENSION?	                 */
	/*---------------------------------------------------------------*/
	size_t ext = client->getFullPath().rfind(".");
	if (ext != std::string::npos)
	{
		size_t extend = client->getFullPath().size();
		std::string extension = client->getFullPath().substr(ext, (extend - ext));
		/*---------------------------------------------------------------*/
		/*                 LOOKING FOR STATIC EXTENSION                  */
		/*---------------------------------------------------------------*/
		if (!this->extpath.empty() && this->extpath.find(extension) != this->extpath.end())
		{
			this->_extensionPath = extension;
			this->_status_code = 0;
			return (this->currentConfig->getimHere() + this->currentConfig->getRoot()
				+ this->extpath[extension] + client->getFullPath());
		}
		/*---------------------------------------------------------------*/
		/*                   LOOKING FOR CGI EXTENSION                   */
		/*---------------------------------------------------------------*/
		this->_is_cgi = 0;
		std::map<std::string, std::string> cgi_ext = this->currentConfig->getPathInfoCgi();		
		if (!cgi_ext.empty() && cgi_ext.find(extension) != this->extpath.end())
		{
			this->_extensionPath = extension;
			this->_status_code = 0;
			this->_is_cgi = 1;
			this->_executer_cgi = cgi_ext[extension];
			return (this->currentConfig->getimHere() + this->currentConfig->getRoot()
				+ CGI_FILES + client->getFullPath());
		}
	}
	/*---------------------------------------------------------------*/
	/*                       PAGE NOT FOUND                          */
	/*---------------------------------------------------------------*/
	log("Extension of the files was not recognize.", 1);
	return (findErrorPage(400));
}

std::string	Server::findErrorPage(int code)
{
	std::string absolutPath;
	if (this->currentConfig->getErrorPageLocation() == "")
		absolutPath = this->currentConfig->getimHere()
					+ DEFAULT_PATH_ERROR
					+ this->currentConfig->getErrorPage(code);
	else
	{
		absolutPath = this->currentConfig->getimHere()
					+ this->currentConfig->getRoot()
					+ this->currentConfig->getErrorPageLocation()
					+ this->currentConfig->getErrorPage(code);
	}
	return (absolutPath);
}

void   	Server::log(std::string error, int type)
{
	std::time_t t = std::time(NULL);
    std::tm* local_time = std::localtime(&t);
	std::stringstream ss;

	if (local_time->tm_hour < 10)
		ss << "0" << local_time->tm_hour << ":";
	else
		ss << local_time->tm_hour << ":";
	if (local_time->tm_min < 10)
		ss << "0" << local_time->tm_min << ":";
	else
		ss << local_time->tm_min << ":";
	if (local_time->tm_sec < 10)
		ss << "0" << local_time->tm_sec;
	else
    	ss << local_time->tm_sec;

	std::string hour(ss.str());
    if (_log && _log->is_open()) {
        if (type == 1)
			*_log << "\t[INFO][" << hour << "] : " << error << std::endl;
	else if (type == 2)
		*_log << "\t[ERROR][" << hour << "] : " << error << std::endl;
	else if (type == 3)
		*_log << "[INFO][" << hour << "] : " << error << std::endl;
    } else {
        std::cerr << "Log error: Log stream is null or not open." << std::endl;
    }
}

void 	Server::set_nonblocking(int sockfd) 
{
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1)
	{
		log("fnctl failed.", 2);
		return ;
    }
    flags |= O_NONBLOCK;
    if (fcntl(sockfd, F_SETFL, flags) == -1) 
	{
		log("fnctl failed.", 2);
		return ;
    }
}

void	Server::error(std::string errorType)
{
	throw(std::runtime_error(errorType));
}

/*----------------------------------------------------------------------------*/
/*                             INITIALISATION                                 */
/*----------------------------------------------------------------------------*/

std::map<std::string, std::string>	Server::createExtPath()
{
	std::map<std::string, std::string> extPath;

	extPath[".html"] = HTML_FILES;
	extPath[".css"] = CSS_FILES;
	extPath[".js"] = JS_FILES;
	extPath[".json"] = JSON_FILES;
	extPath[".jpg"] = IMAGE_FILES;
	extPath[".jpeg"] = IMAGE_FILES;
	extPath[".png"] = IMAGE_FILES;
	extPath[".gif"] = IMAGE_FILES;
	extPath[".bmp"] = IMAGE_FILES;
	extPath[".ico"] = ICONS_FILES;
	extPath[".webp"] = IMAGE_FILES;
	extPath[".svg"] = IMAGE_FILES;
	extPath[".mp4"] = VIDEO_FILES;
	extPath[".webm"] = VIDEO_FILES;
	extPath[".avi"] = VIDEO_FILES;
	extPath[".mp3"] = AUDIO_FILES;
	extPath[".pdf"] = PDF_FILES;
	extPath[".xml"] = XML_FILES;
	extPath[".ttf"] = FONT_FILES;
	extPath[".woff"] = FONT_FILES;
	extPath[".woff2"] = FONT_FILES;
	extPath[".csv"] = CSV_FILES;
	return (extPath);
}

std::map<std::string, std::string>	Server::createMimePath()
{
	std::map<std::string, std::string> mimePath;

	mimePath[".sh"] = "text/html";
	mimePath[".py"] = "text/html";
	mimePath[".html"] = "text/html";
	mimePath[".css"] = "text/css";
	mimePath[".js"] = "application/javascript";
	mimePath[".json"] = "application/json";
	mimePath[".jpg"] = "image/jpeg";
	mimePath[".jpeg"] = "image/jpeg";
	mimePath[".png"] = "image/png";
	mimePath[".gif"] = "image/gif";
	mimePath[".bmp"] = "image/bmp";
	mimePath[".ico"] = "images/icons";
	mimePath[".webp"] = "image/webp";
	mimePath[".svg"] = "image/svg+xml";
	mimePath[".mp4"] = "video/mp4";
	mimePath[".webm"] = "video/webm";
	mimePath[".avi"] = "video/x-msvideo";
	mimePath[".mp3"] = "audio/mpeg";
	mimePath[".pdf"] = "application/pdf";
	mimePath[".xml"] = "application/xml";
	mimePath[".ttf"] = "font/ttf";
	mimePath[".woff"] = "font/woff";
	mimePath[".woff2"] = "font/woff2";
	mimePath[".csv"] = "text/csv";
	return (mimePath);
}
