/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:51:56 by sguillot          #+#    #+#             */
/*   Updated: 2024/08/29 18:04:45 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

/* -------------------------------------------------------------------------- */
/*                                    DEFINE                                  */
/* -------------------------------------------------------------------------- */

#define HELP_INFOS "usage : ./webserv [<file.conf] [--help]\n\n" \
    "informations required in the configuration file (token must be separated by spaces or tabulations):\n\n" \
    "server {\n" \
    "\tlisten <port number>\n\n" \
    "\tserver_name <server name>\n\n" \
    "\t error_page <error code> <error html file>\n\n" \
    "\t client_max_body_size <size in M>\n\n" \
    "\tlocation /<page name> {\n\t\tallow_methods <methods allowed>\n\t\tautoindex <on/off>\n\t}\n" \
    "}"

/* -------------------------------------------------------------------------- */
/*                                    INCLUDES                                */
/* -------------------------------------------------------------------------- */

#include <iostream>
#include <vector>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <functional>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/epoll.h>
#include <algorithm>
#include <list>
#include <netdb.h>
#include <stdlib.h>
#include <map>
#include <string_view>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <signal.h>

#define MAX_EVENTS 10
#define BUFFER_SIZE 4096

#include "ServerConfiguration.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Cgi.hpp"

#define SIGNAL 2
extern int g_signal;

/* -------------------------------------------------------------------------- */
/*                                    STRUCTURES                              */
/* -------------------------------------------------------------------------- */

typedef struct s_parsing
{
    int     fd;
    bool    parsing_started_server;
    bool    parsing_started_brace;
    bool    parsing_ended;
}   t_parsing;

typedef struct s_data
{
    ServerConfiguration *p_server_configuration;
    t_parsing   parsing;
}   t_data;

/* -------------------------------------------------------------------------- */
/*                                    UTILS                                   */
/* -------------------------------------------------------------------------- */

/* error.cpp */
bool    ft_error(t_data *data, std::string str);

/* free_all.cpp */
void    ft_free_all(t_data *data);

/* init.cpp */
void    ft_init(t_data *data, ServerConfiguration *server_configuration);

/* utils.cpp */
int     ft_strcmp(const char *s1, const char *s2);

/* -------------------------------------------------------------------------- */
/*                                    PARSING                                 */
/* -------------------------------------------------------------------------- */

/* parsing_g.cpp */
bool    parsing_g(t_data *data, int argc, char **argv);

/* read_configuration.cpp */
bool    readConfigurationFile(t_data *data, const char *arg);

/* tokenizer.cpp */
bool    ft_tokenizer(t_data *data, const std::string &line);

/* set_error_page.cpp */
bool    ft_set_error_page_param(t_data *data, std::vector<std::string> tokens);

/* set_host_name.cpp */
bool    ft_set_host_name_param(t_data *data, std::vector<std::string> tokens);

/* set_listen.cpp */
bool    ft_set_listen_param(t_data *data, std::vector<std::string> tokens);

/* set_location.cpp */
bool    ft_set_location_param(t_data *data, std::vector<std::string> tokens);

/* set_max_body.cpp */
bool    ft_set_client_max_body_param(t_data *data, std::vector<std::string> tokens);

/* set_server_name.cpp */
bool    ft_set_server_name_param(t_data *data, std::vector<std::string> tokens);

/* set_tokens.cpp */
bool    ft_set_tokens(t_data *data, std::vector<std::string> tokens);
bool    ft_invalid_line(t_data *data, std::vector<std::string> tokens);

/* -------------------------------------------------------------------------- */
/*                                    EXEC                                    */
/* -------------------------------------------------------------------------- */

/* exec_g.cpp */
bool    exec_g(ServerConfiguration *inf);

#endif