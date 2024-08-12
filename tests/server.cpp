#include "server.hpp"

Server::Server(char *port)
{
	this->_socket = new ListeningSocket(port);

	if ((this->_epoll_fd = epoll_create1(0)) == -1)
	{
		close(this->_socket->getSocket_fd());
		throw(std::runtime_error("Error: epoll_fd creation failed"));
	}

	this->_event.events = EPOLLIN;
	this->_event.data.fd = this->_socket->getSocket_fd();

	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, this->_socket->getSocket_fd(), &this->_event) == -1)
	{
		close(this->_epoll_fd);
		close(this->_socket->getSocket_fd());
		delete(this->_socket);
		throw(std::runtime_error("Error: epoll_ctl creation failed"));
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
			close(this->_epoll_fd);
			close(this->_socket->getSocket_fd());
			delete(this->_socket);
			throw(std::runtime_error("Error: epoll_wait failed"));
		}

		for (int i = 0; i < nfds; ++i)
		{
			if (this->_events[i].data.fd == this->_socket->getSocket_fd())
			{
				this->_connexion_fd = accept(this->_socket->getSocket_fd(), NULL, NULL);
				if (this->_connexion_fd == -1)
				{
					close(this->_epoll_fd);
					close(this->_socket->getSocket_fd());
					delete(this->_socket);
					throw(std::runtime_error("Error: accept failed"));
				}

				this->_event.events = EPOLLIN | EPOLLET;
				this->_event.data.fd = this->_connexion_fd;

				if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, this->_connexion_fd, &this->_event) == -1)
				{
					close(this->_connexion_fd);
					close(this->_epoll_fd);
					close(this->_socket->getSocket_fd());
					delete(this->_socket);
					throw(std::runtime_error("Error: epoll_ctl failed"));
				}
			}
			else
			{
				handle_client();
			}
		}
	}
}

std::string Server::getFileContent(const std::string &path)
{
	std::ifstream file(path.c_str());
	std::ostringstream oss;

	if (!file.is_open())
	{
		return("");
	}

	oss << file.rdbuf();
	return(oss.str());
}

std::string Server::findContent(const std::string &receivedLine)
{
	size_t path_start = receivedLine.find('/');
	if (path_start == std::string::npos)
	{
		close(this->_connexion_fd);
		close(this->_epoll_fd);
		close(this->_socket->getSocket_fd());
		delete(this->_socket);
		throw(std::runtime_error("Error: path_start failed"));
	}

	size_t path_end = receivedLine.find(' ', path_start);
	if (path_end == std::string::npos)
	{
		close(this->_connexion_fd);
		close(this->_epoll_fd);
		close(this->_socket->getSocket_fd());
		delete(this->_socket);
		throw(std::runtime_error("Error: path_end failed"));
	}
	this->_path = receivedLine.substr(path_start, path_end - path_start);

	if (this->_path == "/")
	{
		return(getFileContent("../www/html/index.html"));
	}
	else if (this->_path.compare(this->_path.size() - 5, 5, ".html") == 0)
	{
		return(getFileContent("../www/html" + this->_path));
	}
	else if (this->_path.compare(this->_path.size() - 4, 4, ".css") == 0)
	{
		return(getFileContent("../www/styles" + this->_path));
	}

	return(getFileContent("../www/html/errors/400.html"));
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
		close(this->_connexion_fd);
		close(this->_epoll_fd);
		close(this->_socket->getSocket_fd());
		delete(this->_socket);
		throw(std::runtime_error("Error: find method failed"));
	}
	return(this->_method);
}

void Server::handle_client()
{
	int n;
	memset(this->received_line, 0, sizeof(this->received_line));
	n = read(this->_connexion_fd, this->received_line, sizeof(this->received_line) - 1);
	if (n < 0)
	{
		close(this->_connexion_fd);
		close(this->_epoll_fd);
		close(this->_socket->getSocket_fd());
		delete(this->_socket);
		throw(std::runtime_error("Error: read handle client failed"));
	}

	std::string received_line_cpy(this->received_line);
	this->_method = findMethod(received_line_cpy);
	std::string get_content = findContent(received_line_cpy);

	if (this->_method == "GET")
	{
		snprintf(this->socket_buffer, sizeof(this->socket_buffer),
				 "HTTP/1.0 200 OK\r\n\r\n%s",
				 get_content.c_str());
	}
	else if (this->_method == "POST")
	{
		snprintf(this->socket_buffer, sizeof(this->socket_buffer),
				 "HTTP/1.0 200 OK\r\n\r\nReceived POST request\n");
	}
	else if (this->_method == "DELETE")
	{
		snprintf(this->socket_buffer, sizeof(this->socket_buffer),
				 "HTTP/1.0 200 OK\r\n\r\nReceived DELETE request\n");
	}
	else
	{
		snprintf(this->socket_buffer, sizeof(this->socket_buffer),
				 "HTTP/1.0 400 Bad Request\r\n\r\n%s",
				 get_content.c_str());
	}
	write(this->_connexion_fd, this->socket_buffer, strlen(this->socket_buffer));
	close(this->_connexion_fd);
}

Server::~Server()
{
	close(this->_epoll_fd);
	close(this->_socket->getSocket_fd());
	delete(this->_socket);
}
