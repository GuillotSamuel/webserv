/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <mmahfoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:27:50 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/09/10 15:04:35 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "server.hpp"
#include "webserv.hpp"
std::ofstream* Server::_log = NULL;
/*----------------------------------------------------------------------------*/
/*                               CONSTRUCTOR                                  */
/*----------------------------------------------------------------------------*/

Server::Server(int argc, char **argv)
{
	_log = new std::ofstream("logfile.log", std::ios::out);
	if (!_log->is_open())
	{		
    	std::cerr << "Failed to open log file" << std::endl;
	}
	parsing_g(argc, argv);
	log("Starting Server.", 3);
	// TEST
	for (std::vector<ServerConfiguration>::iterator it = tab_serv.begin(); it != tab_serv.end(); ++it)
	{
		std::cout << *it << std::endl;
	}
	// TEST
	this->tab_list = new ListeningSocket*[tab_serv.size()];
	creatMultiListenPort();
	this->_connexion_fd = -1;
	this->_epoll_fd = -1;
	this->extpath = createExtPath();
	this->mimePath = createMimePath();
	memset(&_address, 0, sizeof(struct sockaddr_in));
	memset(&_clientAdress, 0, sizeof(struct sockaddr));
	memset(&_event, 0, sizeof(struct epoll_event));
	memset(received_line, 0, BUFFER_SIZE);
	memset(socket_buffer, 0, BUFFER_SIZE);
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
	std::map<ListeningSocket*, ServerConfiguration*>::iterator it = _config.begin();
	for (; it != this->_config.end(); it++)
	{
		this->_event.events = EPOLLIN;
		this->_event.data.fd = it->first->getSocket_fd();

		if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, it->first->getSocket_fd(), &this->_event) == -1)
		{
			log("Server failed to add the socket file descriptor to his instance of Epoll.", 2);
		}
		log("Epoll_ctl successfully add the socket file descriptor to Epoll instance.", 1);
	}
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
		ServerConfiguration *serv;
		for (int i = 0; i < nfds; i++)
		{
			std::map<ListeningSocket *, ServerConfiguration *>::iterator it = _config.end();
			for (int j = 0; j < (int)_config.size(); j++)
			{
				if (this->_events[i].data.fd == this->tab_list[j]->getSocket_fd())
				{
					it = _config.find(this->tab_list[j]);
					break;
				}
			}
			if (it == _config.end())
			{
				if (this->_events[i].events & EPOLLIN && serv != NULL)
					handle_client(*serv);

				else if (this->_events[i].events & (EPOLLRDHUP | EPOLLHUP)) {
					if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_DEL, this->_events->data.fd, NULL) == -1)
						log("Epoll_ctl failed.", 2);

					close(this->_events->data.fd);
					log("Client has been successfuly closed.", 1);
					continue;
				}
				else
					log("Inexpected event has been detected.", 2);
			}
			else
			{
				if (this->_events[i].data.fd == it->first->getSocket_fd())
				{
					serv = it->second;
					socklen_t client_addrlen = sizeof(this->_clientAdress);
					this->_connexion_fd = accept(it->first->getSocket_fd(), (struct sockaddr *)&this->_clientAdress, &client_addrlen);
					if (this->_connexion_fd == -1)
					{
						log("The call to accept function failed for unknown reason.", 2);
					}
					log("[NEW REQUEST]", 3);
					log("Server did accept the connection.", 1);

					set_nonblocking(this->_connexion_fd);

					this->_event.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP;
					this->_event.data.fd = this->_connexion_fd;
					if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, this->_connexion_fd, &this->_event) == -1)
						log("Epoll_ctl failed.", 2);
				}
			}
		}
	}
}

/*
-Parsing request, download file if needed 
and chose what method to use
*/
void Server::handle_client(ServerConfiguration serv)
{
	Client *client = new Client();

	char client_ip[INET_ADDRSTRLEN];
	
	memset(client_ip, 0, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &this->_address.sin_addr, client_ip, INET_ADDRSTRLEN);
	std::string ipAdress(client_ip);
	client->setIpAddress(ipAdress);
	
	std::string receivedLine = readRequest(client);
	if (receivedLine == "")
	{
		delete client;
		log("Server could not read anything from client. The client will be remove.", 2);
		return ;
	}
	std::string filePath = findPath(receivedLine);
	if (client->getContentLength() != "")
		dlFile(receivedLine, client);

	if (client->getMethod() == "GET")
	{
		ft_get(filePath);
		delete client;
	}
	else if (client->getMethod() == "POST")
	{
		ft_post(*client, filePath, &serv);
		delete client;
	}
	else if (client->getMethod() == "DELETE")
	{
		ft_delete();
		delete client;
	}
	else
	{
		ft_badRequest();
		delete client;
	}

	write(this->_connexion_fd, this->socket_buffer, strlen(this->socket_buffer));
	close(this->_connexion_fd);
	log("Closing the connection with the client.", 1);
}

/*response to a GET request*/
void Server::ft_get(std::string filePath) // a revoir
{
	log("Server's receive a GET request.", 1);
	std::string content = readFileContent(filePath);
	std::string response = "";
	if (content.empty())
	{
		log("The file requested \"" + filePath + "\" was found empty.", 1);
		content = readFileContent(ERROR_400_PAGE);

		response = "HTTP/1.1 400 Bad Request\r\n";
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
		log("The file requested \"" + filePath + "\" was found.", 1);
		std::string mimeType = getMimeType();

		response = "HTTP/1.1 200 OK\r\n";
		response += "Content-Type: " + mimeType + "\r\n";
		std::ostringstream oss;
		oss << content.size();
		response += "Content-Length: " + oss.str() + "\r\n";
		response += "Connection: close\r\n";
		response += "Server: webserv/1.0\r\n\r\n";
		response += content;
	}

	log("Server's ready to respond.", 1);
	if (write(this->_connexion_fd, response.c_str(), response.size()) == -1)
		log("Server's failed to respond to GET request.", 2);
}

/*response to a POST request*/
void Server::ft_post(Client client, std::string filePath, ServerConfiguration *serv) // a revoir surtout au niveau de la requete
{
	log("Server's receive a POST request.", 1);
	Cgi *cgi = new Cgi();

	std::map<std::string, std::string> tmp = serv->getPathInfoCgi();

	cgi->setPathInfoCgi(&tmp);
	cgi->setPath(filePath.c_str());
	cgi->setEnv(serv, client);
	std::string content = cgi->executeCgi();

	std::string mimeType = getMimeType();

	std::string response = "HTTP/1.1 200 OK\r\n";
	response += "Content-Type: text/html\r\n";
	std::ostringstream oss;
	oss << content.size();
	response += "Content-Length: " + oss.str() + "\r\n";
	response += "Connection: close\r\n";
	response += "Server: webserv/1.0\r\n\r\n";
	response += content;

	log("Server's ready to respond.", 1);
	if (write(this->_connexion_fd, response.c_str(), response.size()) == -1)
		log("Server's failed to respond to POST request.", 2);
	delete cgi;
}

/*response to a DELETE request*/
void Server::ft_delete() // a revoir
{
	log("Server's receive a DELETE request.", 1);
	std::string filePath = findPath(this->_path);

	if (access(filePath.c_str(), F_OK) != 0)
	{
		std::string response = "HTTP/1.1 404 Not Found\r\n";
		response += "Content-Type: text/html\r\n";
		std::string content = "File not found";
		std::ostringstream oss;
		oss << content.size();
		response += "Content-Length: " + oss.str() + "\r\n";
		response += "Connection: close\r\n";
		response += "Server: webserv/1.0\r\n\r\n";
		response += "File not found";

		log("Server's ready to respond.", 1);
		if (write(this->_connexion_fd, response.c_str(), response.size()) == -1)
			log("Server's failed to respond to DELETE request.", 2);
		return;
	}

	if (unlink(filePath.c_str()) == 0)
	{
		std::string response = "HTTP/1.1 204 No Content\r\n";
		response += "Connection: close\r\n";
		response += "Server: webserv/1.0\r\n\r\n";

		log("Server's ready to respond.", 1);
		if (write(this->_connexion_fd, response.c_str(), response.size()) == -1)
			log("Server's failed to respond to DELETE request.", 2);

	}
	else
		log("Server failed to respond at the DELETE request.", 2);
}

/*response to a bad request*/
void Server::ft_badRequest()
{
	std::string content = readFileContent(ERROR_400_PAGE);

	std::string response = "HTTP/1.1 400 Bad Request\r\n";
	response += "Content-Type: text/html\r\n";
	std::ostringstream oss;
    oss << content.size();
    response += "Content-Length: " + oss.str() + "\r\n";
	response += "Connection: close\r\n";
	response += "Server: webserv/1.0\r\n\r\n";
	response += content;

	log("The server did not understand this request.", 1);
	write(this->_connexion_fd, response.c_str(), response.size());
}

//Closing The server using key CTRL /C.
void	Server::closeServer()
{
	log("Shutting down the server properly.", 1);
	int i = 0;
	std::map<ListeningSocket *, ServerConfiguration *>::iterator it = _config.begin();
	while (it != this->_config.end())
	{
		close(it->first->getSocket_fd());
		delete (this->tab_list[i]);
		i++;
		it++;
	}
	this->tab_serv.clear();
	this->_config.clear();
	this->mimePath.clear();
	this->extpath.clear();
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

void	Server::dlFile(std::string receivedLine, Client *client)
{
	std::string file_name;
	std::string body_content;
	size_t filename = receivedLine.find("filename=\"");
	if (filename != std::string::npos)
	{
		filename += 10;
		size_t endFilename  = receivedLine.find("\"", filename);
		if (endFilename != std::string::npos)
		{
			file_name = receivedLine.substr(filename, (endFilename - filename));
		
			size_t body = receivedLine.find("\r\n\r\n", endFilename);
			if (body != std::string::npos)
			{
				body += 4;
				size_t endbody = receivedLine.find(client->getBoundary(), body);
				if (endbody != std::string::npos)
				{
					body_content = receivedLine.substr(body, (endbody - body));
					saveFile(file_name, body_content);
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

std::string	Server::readFileContent(const std::string &path)
{
	std::ifstream file(path.c_str(), std::ios::binary);

	if (!file.is_open())
	{
		log("The file given cannot be found.", 1);
		return ("");
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
		log("Failed to open file " + filename + ".", 1);
    }
}

std::string	Server::getMimeType()
{
	if (this->_path == "/")
		return ("text/html");
	
	if (this->mimePath.find(this->_extensionPath) != this->mimePath.end())
		return (this->mimePath[this->_extensionPath]);

	log("Extension of the file cannot be found.", 1);
	return ("application/octet-stream");
}

std::string	Server::readRequest(Client *client)
{
	int n = recv(this->_connexion_fd, this->received_line, 4096, 0);
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
	
	if (client->getContentLength() != "")
	{
		int len = atoi(client->getContentLength().c_str());

        char *buffer = new char[len + 1];
        memset(buffer, 0, len + 1);
        int total_read = 0;
		while (total_read < len)
        {
            int read = recv(this->_connexion_fd, buffer + total_read, len - total_read, 0);
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
		

		receivedLine.append(buffer, total_read);
    	delete[] buffer;
		std::ofstream file("request.txt", std::ios::binary);
		if (file.is_open()) {
			file << receivedLine;
			file.close();
			log("File is created.", 1);
		} 
		else
			log("Unable to open file.", 2);
	}
	return (receivedLine);
}

std::string Server::findPath(const std::string &receivedLine)
{
	char *path_here = getcwd(NULL, 0);
	std::cout << "path = " << path_here << std::endl; // TEST
	size_t path_start = receivedLine.find('/');
	if (path_start == std::string::npos)
		log("Path_start failed.", 2);

	size_t path_end = receivedLine.find(' ', path_start);
	if (path_end == std::string::npos)
		log("Path_end failed.", 2);
	this->_path = receivedLine.substr(path_start, path_end - path_start);

	if (this->_path == "/")
		return (HTML_FILES + std::string("/index.html"));

	size_t ext = this->_path.rfind(".");
	size_t extend = this->_path.size();
	std::string extension = this->_path.substr(ext, (extend - ext));
	if (this->extpath.find(extension) != this->extpath.end())
	{
		this->_extensionPath = extension;
		return (this->extpath[extension] + this->_path);
	}
	
	log("Extension of the files was not recognize.", 1);
	return (ERROR_400_PAGE);
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

void	Server::creatMultiListenPort()
{
	std::vector<ServerConfiguration>::iterator it = tab_serv.begin();
	int i = 0;
	for(; it != tab_serv.end(); it++)
	{
		this->tab_list[i] = new ListeningSocket(it->getPort(), *it);
		this->_config[this->tab_list[i]] = &(*it);
		i++;
	}
}

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
	extPath[".ico"] = IMAGE_FILES;
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
	extPath[".py"] = CGI_FILES;
	extPath[".sh"] = CGI_FILES;
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
	mimePath[".ico"] = "image/x-icon";
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
