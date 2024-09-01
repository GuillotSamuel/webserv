// #include "server.hpp"
#include "webserv.hpp"

Server::Server(int argc, char **argv)
{
	this->_serv = new ServerConfiguration(argc, argv);
	this->_socket = new ListeningSocket(this->_serv->getPort(), *this->_serv);
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

void Server::startingServer()
{
	if ((this->_epoll_fd = epoll_create(1)) == -1)
	{
		this->_serv->log("Error: epoll_fd creation failed", 2);
	}

	this->_serv->log("Epoll instance creation done.", 1);

	this->_event.events = EPOLLIN;
	this->_event.data.fd = this->_socket->getSocket_fd();

	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, this->_socket->getSocket_fd(), &this->_event) == -1)
	{
		this->_serv->log("Error: epoll_ctl creation failed", 2);
	}

	this->_serv->log("Epoll_ctl done.", 1);
	this->_serv->log("Construcion of the server is now finish and he's ready to listen", 1);
}

void Server::serverExecution()
{
	while (true)
	{
		int nfds = epoll_wait(this->_epoll_fd, this->_events, MAX_EVENTS, -1);
		if (nfds == -1)
		{
			this->_serv->log("epoll_wait failed", 2);
		}
		if (g_signal == SIGNAL)
		{
			this->_serv->log("Closing the server properly.", 1);
			delete this->_serv;
			close(this->_socket->getSocket_fd());
			delete this->_socket;
			this->mimePath.clear();
			this->extpath.clear();
			close(this->_epoll_fd);
			exit(EXIT_SUCCESS);
		}
		for (int i = 0; i < nfds; i++) // anciennement ++i
		{
			if (this->_events[i].data.fd == this->_socket->getSocket_fd())
			{
				socklen_t client_addrlen = sizeof(this->_clientAdress);
				this->_connexion_fd = accept(this->_socket->getSocket_fd(), (struct sockaddr *)&this->_clientAdress, &client_addrlen);
				if (this->_connexion_fd == -1)
				{
					this->_serv->log("Accept failed", 2);
				}
				this->_serv->log("NEW REQUEST", 3);
				this->_serv->log("Server did accept the connection", 1);

				set_nonblocking(this->_connexion_fd);

				this->_event.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP;
				this->_event.data.fd = this->_connexion_fd;
				if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, this->_connexion_fd, &this->_event) == -1)
				{
					this->_serv->log("epoll_ctl failed", 2);
				}
			}
			else if (this->_events[i].events & EPOLLIN)
			{
				handle_client();
			}
			else
				this->_serv->log("Inexpected event coming", 2);
			if (this->_events[i].events & (EPOLLRDHUP | EPOLLHUP))
			{
				if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, this->_events->data.fd, NULL) == -1)
				{
					this->_serv->log("epoll_ctl failed.", 2);
				}
				close(this->_events->data.fd);
				this->_serv->log("Client has been successfuly closed.", 1);
				continue;
			}
		}
	}
}

void Server::set_nonblocking(int sockfd)
{
	int flags = fcntl(sockfd, F_GETFL, 0);
	if (flags == -1)
	{
		this->_serv->log("fnctl failed.", 2);
		return;
	}

	flags |= O_NONBLOCK;

	if (fcntl(sockfd, F_SETFL, flags) == -1)
	{
		this->_serv->log("fnctl failed.", 2);
		return;
	}
	this->_serv->log("Fd is now non-blocking", 1);
}

void Server::handle_client()
{
	Client *client = new Client();

	char client_ip[INET_ADDRSTRLEN];

	memset(client_ip, 0, INET_ADDRSTRLEN);
	inet_ntop(AF_INET, &this->_address.sin_addr, client_ip, INET_ADDRSTRLEN);
	std::string ipAdress(client_ip);
	client->setIpAddress(ipAdress);

	int n = recv(this->_connexion_fd, this->received_line, 4095, 0);
	if (n < 0)
	{
		this->_serv->log("Recv failed", 2);
		return;
	}
	if (n == 0)
	{
		this->_serv->log("The connexion has been interupted", 3);
		return;
	}
	std::string receivedLine(this->received_line);

	client->setInfo(receivedLine);

	if (client->getContentLength() != "")
	{
		int len = atoi(client->getContentLength().c_str());

		char buffer[len + 1];
		memset(buffer, 0, len + 1);
		int read = recv(this->_connexion_fd, buffer, len, 0);
		if (read < 0)
			this->_serv->log("Recv failed", 2);
		if (read == 0)
			this->_serv->log("The connexion has been interupted", 2);
		std::string tmp(buffer);
		receivedLine += tmp;

		std::ofstream file("tmp.txt");
		file << receivedLine;
	}

	std::string filePath = findPath(receivedLine);

	if (client->getMethod() == "GET")
	{
		ft_get(filePath);
		delete client;
	}
	else if (client->getMethod() == "POST")
	{
		ft_post(*client, filePath);
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
}

void Server::ft_get(std::string filePath)
{
	std::string content = readFileContent(filePath);

	if (content.empty())
	{
		this->_serv->log("The file requested was found empty, server's ready to response", 1);
		content = readFileContent(ERROR_400_PAGE);

		std::string response = "HTTP/1.1 400 Bad Request\r\n";
		response += "Content-Type: text/html\r\n";
		std::ostringstream oss;
		oss << content.size();
		response += "Content-Length: " + oss.str() + "\r\n";
		response += "Connection: close\r\n";
		response += "Server: webserv/1.0\r\n\r\n";
		response += content;

		write(this->_connexion_fd, response.c_str(), response.size());
	}
	else
	{
		this->_serv->log("The file requested was found, server's ready to response", 1);
		std::string mimeType = getMimeType();

		std::string response = "HTTP/1.1 200 OK\r\n";
		response += "Content-Type: " + mimeType + "\r\n";
		std::ostringstream oss;
		oss << content.size();
		response += "Content-Length: " + oss.str() + "\r\n";
		response += "Connection: close\r\n";
		response += "Server: webserv/1.0\r\n\r\n";
		response += content;
		write(this->_connexion_fd, response.c_str(), response.size());
	}
}

void Server::ft_post(Client client, std::string filePath)
{
	Cgi *cgi = new Cgi();

	cgi->setPathInfoCgi(this->_serv->getPathInfoCgi());
	cgi->setPath(filePath.c_str());
	cgi->setEnv(this->_serv, client);
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

	write(this->_connexion_fd, response.c_str(), response.size());
	delete cgi;
}

void Server::ft_delete()
{
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

		this->_serv->log("Server's ready to respond", 1);
		write(this->_connexion_fd, response.c_str(), response.size());
		return;
	}

	if (unlink(filePath.c_str()) == 0)
	{
		std::string response = "HTTP/1.1 204 No Content\r\n";
		response += "Connection: close\r\n";
		response += "Server: webserv/1.0\r\n\r\n";

		this->_serv->log("Server's ready to respond", 1);
		write(this->_connexion_fd, response.c_str(), response.size());
	}
	else
	{
		this->_serv->log("Ft_delete failed", 2);
	}
}

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

	this->_serv->log("Server's ready to respond", 1);
	write(this->_connexion_fd, response.c_str(), response.size());
}

std::string Server::readFileContent(const std::string &path)
{
	std::ifstream file(path.c_str(), std::ios::binary);

	if (!file.is_open())
	{
		this->_serv->log("The file given in the request does not exist", 1);
		return ("");
	}

	std::ostringstream oss;
	oss << file.rdbuf();
	return (oss.str());
}

std::string Server::getMimeType()
{
	if (this->_path == "/")
		return ("text/html");

	if (this->mimePath.find(this->_extensionPath) != this->mimePath.end())
		return (this->mimePath[this->_extensionPath]);

	this->_serv->log("Extension of the files was not recognize.", 1);
	return ("application/octet-stream");
}
Server::~Server()
{
	// close(this->_epoll_fd);
	// close(this->_socket->getSocket_fd());
	// delete (this->_socket);
}

/*---------------------------------------------UTILS------------------------------------------------------*/

std::string Server::findPath(const std::string &receivedLine)
{
	size_t path_start = receivedLine.find('/');
	if (path_start == std::string::npos)
		this->_serv->log("Path_start failed", 2);

	size_t path_end = receivedLine.find(' ', path_start);
	if (path_end == std::string::npos)
		this->_serv->log("Path_end failed", 2);
	this->_path = receivedLine.substr(path_start, path_end - path_start);

	if (this->_path == "/")
	{
		return (HTML_FILES + std::string("/index.html"));
	}

	size_t ext = this->_path.rfind(".");
	size_t extend = this->_path.size();
	std::string extension = this->_path.substr(ext, (extend - ext));

	if (this->extpath.find(extension) != this->extpath.end())
	{
		this->_extensionPath = extension;
		return (this->extpath[extension] + this->_path);
	}

	this->_serv->log("Extension of the files was not recognize.", 1);

	return (ERROR_400_PAGE);
}

std::map<std::string, std::string> Server::createExtPath()
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

std::map<std::string, std::string> Server::createMimePath()
{
	std::map<std::string, std::string> mimePath;

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
