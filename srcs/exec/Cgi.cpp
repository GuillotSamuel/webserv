#include "Cgi.hpp"

/*------------------------------------CONSTRUCTOR----------------------------------------*/

Cgi::Cgi()
{
	this->_myEnvp = NULL;
	this->_path = NULL;
	this->_myArgv = NULL;
}
 
//Destructor
Cgi::~Cgi()
{
}

/*------------------------------------METHOD----------------------------------------*/

void		Cgi::setEnv(ServerConfiguration server, Client client)
{
	if (client.getMethod() == "POST")
	{
		this->_env["CONTENT_TYPE"] = std::string("a"); // only for post
		this->_env["CONTENT_LENGTH"] = std::string("b"); // only for post
	}

	//SERVEUR_VAR
	this->_env["SERVER_SOFTWARE"] = std::string("c");
	this->_env["SERVER_NAME"] = server.getServerName();
	this->_env["GATEWAY_INTERFACE"] = std::string("CGI/1.1");

	//REQUEST_VAR
	this->_env["SERVER_PROTOCOL"] = std::string("TCP/IP");
	this->_env["SERVER_PORT"] = server.getPort();
	this->_env["REQUEST_METHOD"] = client.getMethod();
	// this->_env["PATH_INFO"] = serv.getPathInfoCgi();
	this->_env["PATH_TRANSLATED"] = (std::string("d"));
	this->_env["SCRIPT_NAME"] = std::string("e"); //le chemin vers le script executer
	this->_env["QUERY_STRING"] = std::string("f");
	this->_env["REMOTE_HOST"] = std::string("g"); // laisse vide si inconnu
	this->_env["REMOTE_ADDR"] = client.getIpAdress();
	// this->_env["AUTH_TYPE"] = ;
	// this->_env["REMOTE_USER"] = ;
	// this->_env["REMOTE_IDENT"] = ;

	//CLIENT_VAR
	this->_env["HTTP_ACCEPT"] = std::string("h");
	this->_env["HTTP_ACCEPT_LANGUAGE"] = std::string("i");
	this->_env["HHTP_USER_AGENT"] = std::string("j");
	this->_env["HTTP_COOKIE"] = std::string("k");
	this->_env["HTTP_REFERER"] = std::string("l");
}


char		**Cgi::conversionEnvFunc()
{
	this->_myEnvp = (char**)malloc((this->_env.size() + 1) * sizeof(char *));

	std::map<std::string, std::string>::const_iterator it = this->_env.begin();
	for (int i = 0; it != this->_env.end(); it++, i++)
	{
		std::string tmp = it->first + "=" + it->second;
		this->_myEnvp[i] = strdup(tmp.c_str());
	}
	return (this->_myEnvp);
}

std::string	Cgi::executeCgi()
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

		char **argv = this->createArgv();
		if (argv == NULL)
			exit(EXIT_FAILURE);
		execve(argv[0], argv, this->conversionEnvFunc());

		error("Error: execve cgi failed");
	}
	close(pipefd[1]);

	char buffer[BUFFER_SIZE];
	int n = 0;
	while ((n = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
	{
		buffer[n] = '\0';
	}
	close(pipefd[0]);
	waitpid(pid, NULL, 0);
	std::string getContent(buffer);
	return (getContent);
}

char	**Cgi::createArgv()
{

	std::cout << this->_path << std::endl;
	// char *argv[] = {const_cast<char *>("/usr/bin/python3"), const_cast<char *>(this->_path), NULL};
	return (NULL);
	// return (argv);
}

/*--------------------------------ERROR MANAGEMENT------------------------------------*/

void		Cgi::error(std::string errorType)
{
	throw(std::runtime_error(errorType));
}

void Cgi::setPath(const char *path)
{
	this->_path = path;
}