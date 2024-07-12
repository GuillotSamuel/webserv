/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_tokens.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 16:09:51 by sguillot          #+#    #+#             */
/*   Updated: 2024/07/12 19:04:12 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static bool ft_invalid_line(t_data *data, std::vector<std::string> tokens)
{
    std::string error_message = "invalid line (no lines allowed after server configurations) [./webserv --help]: ";
    for (int i = 0; (long unsigned int)i < tokens.size(); i++)
    {
        error_message += tokens[i];
        error_message += " ";
    }
    return (ft_error(data, error_message));
}

static bool ft_set_listen_param(t_data *data, std::vector<std::string> tokens)
{
    if (tokens.size() != 2)
    {
        return (ft_invalid_line(data, tokens));
    }
    else if (data->p_server_configuration->getPort() != -1)
    {
        return (ft_error(data, "listen defined many times in configurations : "
            + tokens[0] + " " + tokens[1]));
    }
    
    const std::string& str = tokens[1];

    if (str.empty() || str[str.size() - 1] != ';')
    {
        return ft_error(data, "invalid argument (listen): " + tokens[1]);
    }

    for (size_t i = 0; i < str.size() - 1; ++i)
    {
        if (!std::isdigit(str[i]))
        {
            return ft_error(data, "non-numeric character in port number: " + str);
        }
    }

    const char* num_str = str.substr(0, str.size() - 1).c_str();
    char* end;
    long num = std::strtol(num_str, &end, 10);
    
    if (*end != '\0')
    {
        return ft_error(data, "invalid argument (listen): " + str);
    }
    else if (num < 0 || num > 65535)
    {
        return ft_error(data, "out of bounds port (listen argument): " + tokens[1]);
    }
    else if (num <= 1023 /* Critical system 0-1023 */
        || num == 1080 /* SOCKS Proxy 1080 */
        || (num >= 1433 && num <= 1434) /* Microsoft SQL server 1433-1434 */
        || num == 1521 /* Oracle Database 1521 */
        || num == 1723 /* PPTP (Point-to-Point Tunneling Protocol) 1723 */
        || num == 3306 /* MySQL 3306 */
        || num == 5432 /* PostgreSQL 5432 */
        || (num >= 5900 && num <= 5901) /* VNC (Virtual Network Computing) 5900-5901 */
        || num == 6379 /* Redis 6379 */
        || (num >= 6660 && num <= 6669) /* IRC 6660-6669 */
        || num == 27017 /* MongoDB 27017 */)
    {
        return (ft_error(data, "forbiden port (listen argument): " + tokens[1]));
    }
    
    data->p_server_configuration->setPort(tokens[1].substr(0, tokens[1].size() - 1));
    return (true);
}

static bool ft_is_ip_address(const std::string& str)
{
    if (str.size() < 7 || str.size() > 15
        || std::strncmp("10", str.c_str(), 2) == 0
        || std::strncmp("192.168", str.c_str(), 7) == 0
        || std::strncmp("0.0.0.0", str.c_str(), 7) == 0
        || std::strncmp("255.255.255.255", str.c_str(), 15) == 0)
    {
        return (false);
    }

    int num = 0;
    int point_counter = 0;
    int segment_length = 0;

    for (size_t i = 0; i < str.size(); ++i)
    {
        char ch = str[i];
        if (ch == '.')
        {
            if (num < 0 || num > 255 || segment_length == 0 || segment_length > 3)
            {
                return (false);
            }
            num = 0;
            segment_length = 0;
            ++point_counter;
        }
        else if (ch >= '0' && ch <= '9')
        {
            num = num * 10 + (ch - '0');
            ++segment_length;
        }
        else
        {
            return (false);
        }
    }

    if (num < 0 || num > 255 || point_counter != 3
        || segment_length == 0 || segment_length > 3)
    {
        return (false);
    }

    return (true);
}


static bool ft_set_host_name_param(t_data *data, std::vector<std::string> tokens) {
    if (tokens.size() != 2) {
        return ft_invalid_line(data, tokens);
    }

    const std::string& str = tokens[1];

    if (str.empty() || str[str.size() - 1] != ';') {
        return ft_error(data, "invalid argument (listen): " + tokens[1]);
    }
    
    if (!ft_is_ip_address(tokens[1].substr(0, tokens[1].size() - 1))) {
        return ft_invalid_line(data, tokens);
    }

    data->p_server_configuration->setHostName(tokens[1].substr(0, tokens[1].size() - 1));
    return true;
}


static bool ft_set_server_name_param(t_data *data, std::vector<std::string> tokens)
{
    (void)data;
    (void)tokens;
    return (true);
}

static bool ft_set_error_page_param(t_data *data, std::vector<std::string> tokens)
{
    (void)data;
    (void)tokens;
    return (true);
}

static bool ft_set_client_max_body_param(t_data *data, std::vector<std::string> tokens)
{
    (void)data;
    (void)tokens;
    return (true);
}

static bool ft_set_location_param(t_data *data, std::vector<std::string> tokens)
{
    (void)data;
    (void)tokens;
    return (true);
}

static bool ft_param_set_tokens(t_data *data, std::vector<std::string> tokens)
{
    int nb_param = 6;
    
    std::string param_array[nb_param] = {"listen", "host_name", "server_name", "error_page",
        "client_max_body", "location"};
    
    bool (*param_functions[nb_param])(t_data *, const std::vector<std::string>) = {
        &ft_set_listen_param,
        &ft_set_host_name_param,
        &ft_set_server_name_param,
        &ft_set_error_page_param,
        &ft_set_client_max_body_param,
        &ft_set_location_param
    };

    for (int i = 0; i < nb_param; i++)
    {
        if (tokens[0] == param_array[i])
        {
            return (param_functions[i](data, tokens));
        }
    }
    std::string error_message = "unknown parameter: " + tokens[0];
    return (ft_error(data, error_message));
}

static bool ft_start_set_tokens(t_data *data, std::vector<std::string> tokens)
{
        if (tokens.size() == 1 && data->parsing.parsing_started_server == false && tokens[0] == "server")
        {
            data->parsing.parsing_started_server = true;
        }
        else if ((tokens.size() == 1 && data->parsing.parsing_started_server == false && tokens[0] == "server{")
            || (tokens.size() == 2 && data->parsing.parsing_started_server == false && tokens[0] == "server" && tokens[1] == "{"))
        {
            data->parsing.parsing_started_server = true;
            data->parsing.parsing_started_brace = true;
        }
        else if (tokens.size() == 1 && data->parsing.parsing_started_server == true && data->parsing.parsing_started_brace == false && tokens[0] == "{")
        {
            data->parsing.parsing_started_brace = true;
        }
        else
        {
            std::string error_message = "invalid line [./webserv --help]: ";
            for (int i = 0; (long unsigned int)i < tokens.size(); i++)
            {
                error_message += tokens[i];
                error_message += " ";
            }
            return (ft_error(data, error_message));
        }
        return (true);
}

bool    ft_set_tokens(t_data *data, std::vector<std::string> tokens)
{
    /* START OF TEST */
    std::string test;
    for (int i = 0; (long unsigned int)i < tokens.size(); i++)
    {
        test += tokens[i];
        test += "|";
    }
    std::cout << test << std::endl;
    /* END OF TEST */
    
    if (!tokens.empty())
    {
        if (data->parsing.parsing_ended == true)
        {
            return (ft_invalid_line(data, tokens));
        }
        else if ((data->parsing.parsing_started_server == false || data->parsing.parsing_started_brace == false))
        {
            return (ft_start_set_tokens(data, tokens));
        }
        else if ((data->parsing.parsing_started_server == true && data->parsing.parsing_started_brace == true) && tokens.size() >= 1 && tokens[0] != "}")
        {
            return (ft_param_set_tokens(data, tokens));
        }
        else if ((data->parsing.parsing_started_server == true && data->parsing.parsing_started_brace == true) && tokens.size() >= 1 && tokens[0] == "}")
        {
            data->parsing.parsing_ended = true;
            return (true);
        }
    }
    return (true);
}

