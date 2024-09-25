/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:25:35 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/09/25 12:26:47 by mmahfoud         ###   ########.fr       */
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
	this->_env["SERVER_NAME"] = *server->getServerName().begin();
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

		char **argv = (char **)malloc(3 * sizeof(char *));
		argv[0] = strdup((char *)this->_executer.c_str());
		argv[1] = strdup((char *)this->_path);
		argv[2] = NULL;

		char **envp = this->conversionEnvFunc();

		if (execve(argv[0], argv, envp) == -1)
		{
			error("Error: execve cgi failed");
			freeTab(&argv);
			freeTab(&envp);
		}
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

void	Cgi::setExecuter(std::string executer)
{
	this->_executer = executer;
}

void Cgi::freeTab(char ***tab)
{
	int	i;

	i = 0;
	while (*tab[i])
	{
		free (*tab[i]);
		i++;
	}
	free(*tab);
}
