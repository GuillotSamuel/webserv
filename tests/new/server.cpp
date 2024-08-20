#include "include.hpp"

Server::Server(char *port, char **envp)
{
	this->_socket = new ListeningSocket(port);
	this->_envp = envp;
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
				this->_connexion_fd = accept(this->_socket->getSocket_fd(), NULL, NULL);
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
	int n;
	memset(this->received_line, 0, sizeof(this->received_line));
	n = read(this->_connexion_fd, this->received_line, sizeof(this->received_line) - 1);
	if (n < 0)
	{
		error("Error: read handle client failed");
		exit(EXIT_FAILURE);
	}

	std::string received_line_cpy(this->received_line);
	this->_method = findMethod(received_line_cpy);
	std::string filePath = findPath(received_line_cpy);

	if (this->_method == "GET")
	{
		ft_get(filePath);
	}
	else if (this->_method == "POST")
	{
		ft_post(received_line);
	}
	else if (this->_method == "DELETE")
	{
		ft_delete();
	}
	else
	{
		ft_badRequest(filePath);
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
		return (readFileContent(HTML_FILES + std::string("/index.html")));
	}
	else if (this->_path.compare(this->_path.size() - 5, 5, ".html") == 0)
	{
		return (readFileContent(HTML_FILES + this->_path));
	}
	else if (this->_path.compare(this->_path.size() - 4, 4, ".css") == 0)
	{
		return (readFileContent(CSS_FILES + this->_path));
	}

	return (readFileContent(ERROR_400_PAGE));
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
		response += "Content-Length: " + std::to_string(content.size()) + "\r\n";
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
		response += "Content-Length: " + std::to_string(content.size()) + "\r\n";
		response += "Connection: close\r\n";
		response += "Server: MyCustomServer/1.0\r\n\r\n";
		response += content;

		write(this->_connexion_fd, response.c_str(), response.size());
	}
}

void Server::ft_post(std::string received_line)
{
	/* 	setFormData(received_line);
		snprintf(this->socket_buffer, sizeof(this->socket_buffer),
				 "HTTP/1.0 200 OK\r\n\r\nReceived POST request\n"); */
}

void Server::ft_delete()
{
	/* 	snprintf(this->socket_buffer, sizeof(this->socket_buffer),
				 "HTTP/1.0 200 OK\r\n\r\nReceived DELETE request\n"); */
}

void Server::ft_badRequest(std::string get_content)
{
	/* 	snprintf(this->socket_buffer, sizeof(this->socket_buffer),
				 "HTTP/1.0 400 Bad Request\r\n\r\n%s",
				 get_content.c_str()); */
}

std::string Server::readFileContent(const std::string &path)
{
	std::ifstream file(path.c_str()/* , std::ios::binary */);

std::cout << "PATH : " << path << std::endl; // TEST

	if (!file.is_open())
	{
		std::cout << "\nERROR\n" << std::endl; // TEST
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
	else
		return ("application/octet-stream");
}

Server::~Server()
{
	/* 	close(this->_epoll_fd);
		close(this->_socket->getSocket_fd());
		delete (this->_socket); */
}
