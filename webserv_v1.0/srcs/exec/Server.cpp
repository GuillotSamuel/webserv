/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <mmahfoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:27:50 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/09/30 14:44:54 by mmahfoud         ###   ########.fr       */
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
	memset(&_address, 0, sizeof(struct sockaddr_in));
	memset(&_clientAdress, 0, sizeof(struct sockaddr));
	memset(&_event, 0, sizeof(struct epoll_event));
 	memset(received_line, 0, BUFFER_SIZE);
	memset(socket_buffer, 0, BUFFER_SIZE);
}

/* CREAT SOCKET
- Creat all our listening socket
- Making care to never try to creat socket with the same port twice
*/
void	Server::creatAllListeningSockets()
{
	std::vector<ServerConfiguration>::iterator it = this->tab_serv.begin();
	for (; it < this->tab_serv.end(); it++)
	{
		int boul = 0;
		std::multimap<std::string, std::string> port = it->getPortList();
		std::multimap<std::string, std::string>::iterator itTabPort = port.begin();
		for (; itTabPort != port.end(); itTabPort++)
		{
			std::vector<ListeningSocket*>::iterator itList = _listSockets.begin();
			for (int i = 0; itList < _listSockets.end(); itList++, i++)
			{
				if ((*itList)->getIpAddress() == itTabPort->first
					&& (*itList)->getPortStr() == itTabPort->second)
				{
					boul = 1;
					break;
				}
			}
			if (boul != 1)
			{
				_listSockets.push_back(new ListeningSocket(itTabPort->first, itTabPort->second));
			}
		}
	}
}

/*----------------------------------------------------------------------------*/
/*                              METHOD/SERVER                                 */
/*----------------------------------------------------------------------------*/

// /* STARTING SERVER
// - Creat Epoll Instance 
// - Add Socket_fd to Epoll Instance
// - Watching state : EPOLLIN
// */
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

/* MANAGER ALGORITHM
- Managing every attemp of connexions
*/
void Server::serverExecution()
{
	ListeningSocket *list = NULL;
	while (true)
	{
		int nfds = epoll_wait(this->_epoll_fd, this->_events, MAX_EVENTS, -1);
		if (nfds == -1)
			log("The call to epoll_wait failed.", 2);
		if (g_signal == SIGNAL)
			closeServer();
		for (int i = 0; i < nfds; i++)
		{
			int	sock = 0;
			std::vector<ListeningSocket*>::iterator it = _listSockets.begin();
			for (; it != _listSockets.end(); it++) {
				if (this->_events[i].data.fd == (*it)->getSocket_fd()) {
					sock = (*it)->getSocket_fd();
					list = (*it);
					break;
				}
			}
			if (sock == 0) {
				std::vector<uint32_t>::iterator confd = this->_connexion_fd.begin();
				for(; confd != this->_connexion_fd.end(); confd++) {
					if (*confd == (uint32_t)this->_events[i].data.fd)
						break;
				}
				if (this->_events[i].events & EPOLLIN) {
					inConnexion(list, *confd);
				} else if (this->_events[i].events & (EPOLLRDHUP | EPOLLHUP)) {
					outConnexionClient(*confd);
					list = NULL;
				} else if (this->_events[i].events & EPOLLOUT) {
					outConnexionServer(*confd);
					list = NULL;
				} else
					log("Inexpected event has been detected.", 2);
			} else {
				if (this->_events[i].data.fd == sock) {
					acceptConnexion(sock);
				}
			}
		}
	}
}

/*STEP THREE (EPOLLRDHUP/EPOLLHUP):
-Client close the connexion from his side
-Deleting connexionFD from Epoll Instance
*/
void	Server::outConnexionClient(int connexionFD)
{
	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_DEL, connexionFD, &this->_event) == -1)
	{
		log("Epoll_ctl failed.", 2);
	}
	close(connexionFD);
	log("Connexion has been successfuly closed. ConnexionFD removed from Epoll Instance.", 1);
}

/*STEP THREE (EPOLLOUT):
-Sending response
-Deleting connexionFD from Epoll Instance
*/
void	Server::outConnexionServer(int connexionFD)
{
	send(connexionFD, this->_response.c_str(), this->_response.size(), 0);
	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_DEL, connexionFD, &this->_event) == -1)
	{
		log("Epoll_ctl failed.", 2);
	}
	close(connexionFD);
	log("Connexion has been successfuly closed. ConnexionFD removed from Epoll Instance.", 1);
}

/*STEP TWO:
-Generate response
-Modif of watching state on connexionDF
-Watching state : EPOLLOUT / EPOLLRDHUP / EPOLLHUP
*/
void	Server::inConnexion(ListeningSocket *list, int connexionFD)
{
	handle_client(list, connexionFD);
	this->_event.events = EPOLLOUT | EPOLLRDHUP | EPOLLHUP;
	this->_event.data.fd = connexionFD;
	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_MOD, connexionFD, &this->_event) == -1)
	{
		log("Epoll_ctl failed.", 2);
	}
	log("Got a EPOLLIN attempt.", 1);
}

/*STEP ONE:
-Accept the connexion
-Adding the ConnexionFD to instance Epoll
-Watching state : EPOLLIN
*/
void	Server::acceptConnexion(int sock)
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

/*
-Parsing request, download file if needed 
and chose what method to use
*/
void Server::handle_client(ListeningSocket *list, int current_fd)
{
	Client *client = new Client();
	//recuperation de toute les donnes possible et recherche de server block/location block
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
	getServBlock(client, list);
	if (!this->currentConfig) //TEST
		exit(EXIT_FAILURE);//TEST
	getLocationBlock(client);
	
	Response *response = new Response(client);
	response->setInfo(this->currentConfig, this->_currentLocation);
	this->_response = response->generateResponse();
	delete client;
	delete response;
	log("End of the request.", 1);
}

void	Server::getLocationBlock(Client *client)
{
	//gerer la correspondance exacte
	std::vector<Location> tab = currentConfig->getLocation();
	std::vector<Location>::iterator it = tab.begin();
	for (; it != tab.end(); it++)
	{
		if (client->getPath() == it->getBlockName() && it->getBlockType() == "equal")
		{
			this->_currentLocation = *it;
			return ;
		}
	}
	Location	bestMatch;
	size_t		longestPrefix = 0;
	it = tab.begin();
	for (; it != tab.end(); ++it)
	{
		if (it->getBlockType() == "prefixe")
		{
			std::string blockName = it->getBlockName();
			if (client->getPath().compare(0, blockName.length(), blockName) == 0)
			{
				if (blockName.length() > longestPrefix)
				{
					longestPrefix = blockName.length();
					bestMatch = *it;
				}
			}
		}
	}

	// Si une correspondance par préfixe a été trouvée
	if (bestMatch.getBlockName() != "")
	{
		this->_currentLocation = bestMatch;
		return;
	}
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

/*
	-Adresse IP - Port de la requete
	-le nom de Domaine
	-Block par default
	*/
void	Server::getServBlock(Client *client, ListeningSocket *list)
{
	this->currentConfig = NULL;
	std::vector<ServerConfiguration>::iterator it = this->tab_serv.begin();
	for (; it != this->tab_serv.end(); it++)
	{
		std::multimap<std::string, std::string> portList = it->getPortList();
		std::multimap<std::string, std::string>::iterator it2 = portList.begin();
		for (; it2 != portList.end(); it2++)
		{
			if (it2->first == list->getIpAddress() && it2->second == list->getPortStr())
			{
				this->currentConfig = &(*it);
				return ;
			}
		}
	}
	it = this->tab_serv.begin();
	for (; it < this->tab_serv.end(); it++)
	{
		std::vector<std::string> server_name = it->getServerName();
		std::vector<std::string>::iterator itServ = server_name.begin();
		for (; itServ != server_name.end(); itServ++)
		{
			if (*itServ == client->getPath())
			{
				this->currentConfig = &(*it);
				return ;
			}
		}
	}
	//bloc par default
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
	// cgi->setExecuter(this->_executer_cgi);
	cgi->setPath(filePath.c_str());
	cgi->setEnv(this->currentConfig, client); 
	std::string content = cgi->executeCgi();

	// std::string mimeType = getMimeType(&client);

	std::string response = "HTTP/1.1 200 OK\r\n";
	response += "Content-Type: text/html\r\n";
	std::ostringstream oss;
	oss << content.size();
	response += "Content-Length: " + oss.str() + "\r\n";
	response += "Connection: close\r\n";
	response += "Server: " + *this->currentConfig->getServerName().begin() + "\r\n\r\n";
	response += content;

	this->_response = response;
	delete cgi;
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
					// saveFile(this->currentConfig->getUploadLocation() + file_name, body_content);
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

