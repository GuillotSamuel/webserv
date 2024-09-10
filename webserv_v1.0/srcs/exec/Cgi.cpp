/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:25:35 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/09/08 12:04:30 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

/*----------------------------------------------------------------------------*/
/*                               CONSTRUCTOR                                  */
/*----------------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------------*/
/*                              METHOD/SERVER                                 */
/*----------------------------------------------------------------------------*/

void		Cgi::setEnv(ServerConfiguration *server, Client client)
{
	char *actudir = getcwd(NULL, 0);
	std::string here(actudir, strlen(actudir));
	free(actudir);
	std::string absolutepath = here + this->_path;

	if (client.getMethod() == "POST")
	{
		this->_env["CONTENT_TYPE"] = client.getContentType(); // only for post
		this->_env["CONTENT_LENGTH"] = client.getContentLength(); // only for post
	}

	//SERVEUR_VAR
	this->_env["SERVER_SOFTWARE"] = std::string("Webserv/1.0");
	this->_env["SERVER_NAME"] = server->getServerName();
	this->_env["GATEWAY_INTERFACE"] = std::string("CGI/1.1");

	//REQUEST_VAR
	this->_env["SERVER_PROTOCOL"] = std::string("HTTP/1.1");
	this->_env["SERVER_PORT"] = server->getStrPort();
	this->_env["REQUEST_METHOD"] = client.getMethod();
	this->_env["PATH_INFO"] = this->_path;
	this->_env["PATH_TRANSLATED"] = absolutepath;
	this->_env["SCRIPT_NAME"] = this->_path; //le chemin vers le script executer
	this->_env["QUERY_STRING"] = std::string("");
	this->_env["REMOTE_HOST"] = std::string(""); // laisse vide si inconnu
	this->_env["REMOTE_ADDR"] = client.getIpAdress();

	//CLIENT_VAR
	this->_env["HTTP_ACCEPT"] = client.getAcceptMime();
	this->_env["HTTP_ACCEPT_LANGUAGE"] = client.getAcceptLanguage();
	this->_env["HTTP_USER_AGENT"] = client.getUserAgent();
	this->_env["HTTP_COOKIE"] = std::string("");
	this->_env["HTTP_REFERER"] = client.getReferer();
}


char	**Cgi::conversionEnvFunc()
{
	this->_myEnvp = (char**)malloc((this->_env.size() + 1) * sizeof(char *));

	std::map<std::string, std::string>::const_iterator it = this->_env.begin();
	for (int i = 0; it != this->_env.end(); it++, i++)
	{
		std::string tmp = it->first + "=" + it->second;
		this->_myEnvp[i] = strdup(tmp.c_str());
	}

	int i = this->_env.size();
	this->_myEnvp[i] = NULL;
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
		char **envp = this->conversionEnvFunc();
		// int i;

		// fprintf(stderr, "%s\n%s\n", argv[0], argv[1]);
		// i = 0;
		// while (envp[i])
		// {
		// 	fprintf(stderr, "%s\n", envp[i]);
		// 	i++;
		// }
		execve(argv[0], argv, envp);
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
	char **argv;
	char *here1 = getcwd(NULL, 0);
	std::cout << here1 << std::endl; // TEST
	std::string here(here1, strlen(here1));
	std::string path(this->_path);
	std::string absolutepath = here + path;
	std::string executer = "";


	size_t ext = path.rfind(".");
	size_t extend = path.size();
	std::string extension = path.substr(ext, (extend - ext));

	if ((this->_pathInfoCgi)->find(extension) != this->_pathInfoCgi->end())
		executer = (*_pathInfoCgi)[extension];
	if (executer == "")
	{
		fprintf(stderr, "extension of cgi not recognize\n");
		exit(EXIT_FAILURE);
	}
	argv = (char **) malloc(3 * sizeof(char *));
	argv[0] = strdup(executer.c_str());
	argv[1] = strdup(absolutepath.c_str());
	argv[2] = NULL;
	return (argv);
}
/*----------------------------------------------------------------------------*/
/*                                  UTILS                                     */
/*----------------------------------------------------------------------------*/

void	Cgi::error(std::string errorType)
{
	throw(std::runtime_error(errorType));
}

void	Cgi::setPath(const char *path)
{
	this->_path = path;
}

void	Cgi::setPathInfoCgi(std::map<std::string, std::string> *map)
{
	this->_pathInfoCgi = map;
}
