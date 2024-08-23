// #include "server.hpp"
#include "webserv.hpp"

Server::Server(ServerConfiguration info)
{
	this->_serv = info;
	this->_socket = new ListeningSocket(info.getPort());
	this->_connexion_fd = -1;
	this->_epoll_fd = -1;

	if ((this->_epoll_fd = epoll_create1(0)) == -1)
	{
		error("Error: epoll_fd creation failed");
	}

	this->_event.events = EPOLLIN;
	this->_event.data.fd = this->_socket->getSocket_fd();

	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, this->_socket->getSocket_fd(), &this->_event) == -1)
	{
		error("Error: epoll_ctl creation failed");
	}
	ServerExecution();
}

void Server::ServerExecution()
{
	while (true)
	{
		int nfds = epoll_wait(this->_epoll_fd, this->_events, MAX_EVENTS, -1);
		if (nfds == -1)
		{
			error("Error: epoll_wait failed");
		}
		for (int i = 0; i < nfds; ++i)
		{
			if (this->_events[i].data.fd == this->_socket->getSocket_fd())
			{
				socklen_t client_addrlen = sizeof(this->_clientAdress);
				this->_connexion_fd = accept(this->_socket->getSocket_fd(), (struct sockaddr *)&this->_clientAdress, &client_addrlen);
				if (this->_connexion_fd == -1)
				{
					error("Error: accept failed");
				}

				this->_event.events = EPOLLIN | EPOLLET;
				this->_event.data.fd = this->_connexion_fd;

				if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, this->_connexion_fd, &this->_event) == -1)
				{
					error("Error: epoll_ctl failed");
				}
			}
			else
			{
				handle_client();
			}
		}
	}
}

void Server::error(std::string errorType)
{
	throw(std::runtime_error(errorType));
}

void Server::handle_client()
{
	Client *client = new Client();

	char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &this->_address.sin_addr, client_ip, INET_ADDRSTRLEN);
	std::string ipAdress(client_ip);
	client->setIpAddress(ipAdress);

	int n;
	
	n = 1;
	std::string received_line_cpy = "";
	while (n > 0)
	{
		memset(this->received_line, 0, sizeof(this->received_line));
		if ((n = read(this->_connexion_fd, this->received_line, sizeof(this->received_line) - 1)) <= 0)
		{
			error("Read Failed!");
			return ;
		}
		std::string tmp(this->received_line);
		if (tmp.size() < sizeof(this->received_line))
		{
			received_line_cpy += tmp;
			break;
		}
		received_line_cpy += tmp;
	}

	client->setInfo(received_line_cpy);
	std::cout << *client << std::endl; // TEST
	std::string filePath = findPath(received_line_cpy);

	if (client->getMethod() == "GET")
	{
		ft_get(filePath);
	}
	else if (client->getMethod() == "POST")
	{
		ft_post(*client, filePath);
	}
	else if (client->getMethod() == "DELETE")
	{
		ft_delete();
	}
	else
	{
		ft_badRequest();
	}

	write(this->_connexion_fd, this->socket_buffer, strlen(this->socket_buffer));
	close(this->_connexion_fd);
}

std::string Server::findPath(const std::string &receivedLine)
{
	size_t path_start = receivedLine.find('/');
	if (path_start == std::string::npos)
	{
		error("Error: path_start failed");
	}

	size_t path_end = receivedLine.find(' ', path_start);
	if (path_end == std::string::npos)
	{
		error("Error: path_end failed");
	}
	this->_path = receivedLine.substr(path_start, path_end - path_start);

	if (this->_path == "/")
	{
		return (HTML_FILES + std::string("/index.html"));
	}
	else if (this->_path.compare(this->_path.size() - 5, 5, ".html") == 0)
	{
		return (HTML_FILES + this->_path);
	}
	else if (this->_path.compare(this->_path.size() - 4, 4, ".css") == 0)
	{
		return (CSS_FILES + this->_path);
	}
	else if (this->_path.compare(this->_path.size() - 3, 3, ".js") == 0)
	{
		return (JS_FILES + this->_path);
	}
	else if (this->_path.compare(this->_path.size() - 5, 5, ".json") == 0)
	{
		return (JSON_FILES + this->_path);
	}
	else if ((this->_path.compare(this->_path.size() - 4, 4, ".jpg") == 0) ||
			 (this->_path.compare(this->_path.size() - 5, 5, ".jpeg") == 0) ||
			 (this->_path.compare(this->_path.size() - 4, 4, ".png") == 0) ||
			 (this->_path.compare(this->_path.size() - 4, 4, ".gif") == 0) ||
			 (this->_path.compare(this->_path.size() - 4, 4, ".bmp") == 0) ||
			 (this->_path.compare(this->_path.size() - 4, 4, ".ico") == 0) ||
			 (this->_path.compare(this->_path.size() - 5, 5, ".webp") == 0) ||
			 (this->_path.compare(this->_path.size() - 4, 4, ".svg") == 0))
	{
		return (IMAGE_FILES + this->_path);
	}
	else if (this->_path.compare(this->_path.size() - 4, 4, ".mp4") == 0 ||
			 this->_path.compare(this->_path.size() - 4, 4, ".webm") == 0 ||
			 this->_path.compare(this->_path.size() - 4, 4, ".avi") == 0)
	{
		return (VIDEO_FILES + this->_path);
	}
	else if (this->_path.compare(this->_path.size() - 3, 3, ".mp3") == 0)
	{
		return (AUDIO_FILES + this->_path);
	}
	else if (this->_path.compare(this->_path.size() - 4, 4, ".pdf") == 0)
	{
		return (PDF_FILES + this->_path);
	}
	else if (this->_path.compare(this->_path.size() - 4, 4, ".xml") == 0)
	{
		return (XML_FILES + this->_path);
	}
	else if (this->_path.compare(this->_path.size() - 3, 3, ".ttf") == 0 ||
			 this->_path.compare(this->_path.size() - 4, 4, ".woff") == 0 ||
			 this->_path.compare(this->_path.size() - 5, 5, ".woff2") == 0)
	{
		return (FONT_FILES + this->_path);
	}
	else if (this->_path.compare(this->_path.size() - 3, 3, ".csv") == 0)
	{
		return (CSV_FILES + this->_path);
	}

	return (ERROR_400_PAGE);
}

std::string Server::findMethod(const std::string &receivedLine)
{
	size_t method_end = receivedLine.find(' ');
	if (method_end != std::string::npos)
	{
		this->_method = receivedLine.substr(0, method_end);
	}
	else
	{
		error("Error: find method failed");
	}
	return (this->_method);
}

void Server::ft_get(std::string filePath)
{
	std::string content = readFileContent(filePath);

	if (content.empty())
	{
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
		std::string mimeType = getMimeType(filePath);

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
	cgi->setEnv(this->_serv, client);
	cgi->setPath(filePath.c_str());
	std::string content = cgi->executeCgi();
	
	snprintf(this->socket_buffer, sizeof(this->socket_buffer),
			 "HTTP/1.0 200 OK\r\n\r\n%s", content.c_str());
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

		write(this->_connexion_fd, response.c_str(), response.size());
		return;
	}

	if (unlink(filePath.c_str()) == 0)
	{
		std::string response = "HTTP/1.1 204 No Content\r\n";
		response += "Connection: close\r\n";
		response += "Server: webserv/1.0\r\n\r\n";

		write(this->_connexion_fd, response.c_str(), response.size());
	}
	else
	{
		error("Error: ft_delete failed");
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

	write(this->_connexion_fd, response.c_str(), response.size());
}

std::string Server::readFileContent(const std::string &path)
{
	std::ifstream file(path.c_str(), std::ios::binary);

	if (!file.is_open())
	{
		return ("");
	}

	std::ostringstream oss;
	oss << file.rdbuf();
	return (oss.str());
}

std::string Server::getMimeType(const std::string &path)
{
	if (path.size() > 5 && path.substr(path.size() - 5) == ".html")
		return ("text/html");
	else if (path.size() > 4 && path.substr(path.size() - 4) == ".css")
		return ("text/css");
	else if (path.size() > 3 && path.substr(path.size() - 3) == ".js")
		return ("application/javascript");
	else if (path.size() > 5 && path.substr(path.size() - 5) == ".json")
		return ("application/json");
	else if ((path.size() > 4 && path.substr(path.size() - 4) == ".jpg") ||
			 (path.size() > 5 && path.substr(path.size() - 5) == ".jpeg"))
		return ("image/jpeg");
	else if (path.size() > 4 && path.substr(path.size() - 4) == ".png")
		return ("image/png");
	else if (path.size() > 4 && path.substr(path.size() - 4) == ".gif")
		return ("image/gif");
	else if (path.size() > 4 && path.substr(path.size() - 4) == ".bmp")
		return ("image/bmp");
	else if (path.size() > 4 && path.substr(path.size() - 4) == ".ico")
		return ("image/x-icon");
	else if (path.size() > 4 && path.substr(path.size() - 4) == ".svg")
		return ("image/svg+xml");
	else if (path.size() > 4 && path.substr(path.size() - 4) == ".xml")
		return ("application/xml");
	else if (path.size() > 4 && path.substr(path.size() - 4) == ".pdf")
		return ("application/pdf");
	else if (path.size() > 3 && path.substr(path.size() - 3) == ".mp3")
		return ("audio/mpeg");
	else if (path.size() > 4 && path.substr(path.size() - 4) == ".mp4")
		return ("video/mp4");
	else if (path.size() > 4 && path.substr(path.size() - 4) == ".webm")
		return ("video/webm");
	else if (path.size() > 5 && path.substr(path.size() - 5) == ".webp")
		return ("image/webp");
	else if (path.size() > 3 && path.substr(path.size() - 3) == ".ttf")
		return ("font/ttf");
	else if (path.size() > 4 && path.substr(path.size() - 4) == ".woff")
		return ("font/woff");
	else if (path.size() > 5 && path.substr(path.size() - 5) == ".woff2")
		return ("font/woff2");
	else if (path.size() > 3 && path.substr(path.size() - 3) == ".csv")
		return ("text/csv");
	else if (path.size() > 4 && path.substr(path.size() - 4) == ".avi")
		return ("video/x-msvideo");
	else
		return ("application/octet-stream");
}
Server::~Server()
{
	// close(this->_epoll_fd);
	// close(this->_socket->getSocket_fd());
	// delete (this->_socket);
}