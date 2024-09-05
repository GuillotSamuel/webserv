/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:51:56 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/04 18:27:56 by sguillot         ###   ########.fr       */
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

/* utils.cpp */
int     ft_strcmp(const char *s1, const char *s2);

#endif