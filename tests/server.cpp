// #include "server.hpp"
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

std::string Server::getFileContent(const std::string &path)
{
	std::ifstream file(path.c_str());
	std::ostringstream oss;

	if (!file.is_open())
	{
		return (getFileContent("../www/html/errors/400.html"));
	}

	oss << file.rdbuf();
	return (oss.str());
}

std::string Server::findContent(const std::string &receivedLine)
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
		return (getFileContent("../www/html/index.html"));
	}
	else if (this->_path.compare(this->_path.size() - 5, 5, ".html") == 0)
	{
		return (getFileContent("../www/html" + this->_path));
	}
	else if (this->_path.compare(this->_path.size() - 4, 4, ".css") == 0)
	{
		return (getFileContent("../www/styles" + this->_path));
	}
	else if (is_cgi_script(this->_path.c_str()))
	{
		execute_cgi_script();
		return (this->socket_buffer);
	}

	return (getFileContent("../www/html/errors/400.html"));
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

void Server::ft_get(std::string get_content)
{
	snprintf(this->socket_buffer, sizeof(this->socket_buffer),
			 "HTTP/1.0 200 OK\r\n\r\n%s",
			 get_content.c_str());
}

void Server::ft_post(std::string received_line)
{
	setFormData(received_line);
	snprintf(this->socket_buffer, sizeof(this->socket_buffer),
			 "HTTP/1.0 200 OK\r\n\r\nReceived POST request\n");
}

void Server::ft_delete()
{
	snprintf(this->socket_buffer, sizeof(this->socket_buffer),
			 "HTTP/1.0 200 OK\r\n\r\nReceived DELETE request\n");
}

void Server::ft_badRequest(std::string get_content)
{
	snprintf(this->socket_buffer, sizeof(this->socket_buffer),
			 "HTTP/1.0 400 Bad Request\r\n\r\n%s",
			 get_content.c_str());
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
	std::string get_content = findContent(received_line_cpy);

	if (this->_method == "GET")
	{
		/* if (is_cgi_script(this->_path.c_str()))
		{
			execute_cgi_script();
		}
		else
		{ */
		ft_get(get_content);
/* 		}
 */	}
else if (this->_method == "POST")
{
	/* if (is_cgi_script(this->_path.c_str()))
	{
		execute_cgi_script();
	}
	else
	{ */
	ft_post(received_line);
/* 		}
 */	}
else if (this->_method == "DELETE")
{
	ft_delete();
}
else
{
	ft_badRequest(get_content);
}
write(this->_connexion_fd, this->socket_buffer, strlen(this->socket_buffer));
close(this->_connexion_fd);
}

Server::~Server()
{
	close(this->_epoll_fd);
	close(this->_socket->getSocket_fd());
	delete (this->_socket);
}

void Server::setFormData(std::string receivedLine)
{
	FormData *data = new FormData();
	std::string firstName;
	std::string lastName;
	std::string favoriteColor;
	std::string emailAdress;

	// std::cout << "TEST : " << receivedLine << std::endl; // TEST

	size_t fn = receivedLine.find("first-name=") + 11; // on est sur le char just apres =
	if (fn == std::string::npos)
		error("Error: set Form Data failed");

	size_t endfn = receivedLine.find("&", fn); // on est sur le char &
	if (endfn == std::string::npos)
		error("Error: set Form Data failed");

	firstName = receivedLine.substr(fn, (endfn - fn)); // a partir de = + 1 jusqua &

	size_t ln = receivedLine.find("last-name=", endfn) + 10; // on est sur le char just apres =
	if (ln == std::string::npos)
		error("Error: set Form Data failed");

	size_t endln = receivedLine.find("&", endfn + 1); // on part du &+1 et on est sur le char &
	if (endln == std::string::npos)
		error("Error: set Form Data failed");

	lastName = receivedLine.substr(ln, (endln - ln));

	size_t fc = receivedLine.find("favorite-color=", ln) + 15;
	if (fc == std::string::npos)
		error("Error: set Form Data failed");

	size_t endfc = receivedLine.find("&", endln + 1);
	if (endfc == std::string::npos)
		error("Error: set Form Data failed");

	favoriteColor = receivedLine.substr(fc, (endfc - fc));

	size_t ea = receivedLine.find("email=", fc) + 6;
	if (ea == std::string::npos)
		error("Error: set Form Data failed");

	size_t endea = receivedLine.find("&", endfc + 1);
	if (endea == std::string::npos)
		error("Error: set Form Data failed");

	emailAdress = receivedLine.substr(ea, (endea - ea));
	data->setFirstName(firstName);
	data->setLastName(lastName);
	data->setFavoriteColor(favoriteColor);
	data->setEmailAdress(emailAdress);
	this->my_list.push_back(*data);

	std::cout << *data << std::endl; // TEST
}

void Server::error(std::string errorType)
{
	// if (this->_connexion_fd > 0)
	// {
	// 	if (close(this->_connexion_fd) == -1)
	// 		printf("TEST, un probleme lors de la fermeture du connexion fd\n"); // TEST
	// }
	// if (this->_epoll_fd > 0)
	// {
	// 	if (close(this->_epoll_fd) == -1)
	// 		printf("TEST, un probleme lors de la fermeture du epoll fd\n"); // TEST
	// }
	// if (this->_socket->getSocket_fd() > 0)
	// {
	// 	if (close(this->_socket->getSocket_fd()) == -1)
	// 		printf("TEST, un probleme lors de la fermeture du socket fd\n"); // TEST

	// }
	// if (this->_socket)
	// 	delete (this->_socket);
	throw(std::runtime_error(errorType));
}

std::list<FormData> Server::getList()
{
	return (this->my_list);
}

void Server::execute_cgi_script()
{
	int pipefd[2];
	if (pipe(pipefd) == -1)
	{
		error("Error: pipe creation failed");
	}

	int pid = fork();
	if (pid == -1)
	{
		error("Error: fork cgi failed");
	}
	else if (pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);

		char *argv[] = {const_cast<char *>(this->_path.c_str()), NULL};
		execve(argv[0], argv, this->_envp);

		error("Error: execve cgi failed");
	}
	else
	{
		close(pipefd[1]);

		char buffer[BUFFER_SIZE];
		int n = 0;

		while ((n = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
		{
			buffer[n] = '\0';
			strncat(this->socket_buffer, buffer, sizeof(socket_buffer));
		}
		close(pipefd[0]);
		waitpid(pid, NULL, 0);
	}
}

bool Server::is_cgi_script(const char *path)
{

	std::cout << path << std::endl;
	const char *cgi_extension = ".cgi";
	size_t path_length = std::strlen(path);
	size_t extension_length = std::strlen(cgi_extension);

	if (path_length > extension_length)
	{
		if (std::strcmp(path + path_length - extension_length, cgi_extension) == 0)
		{
			return (true);
		}
	}

	return (false);
}
