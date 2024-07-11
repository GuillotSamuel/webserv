/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_tokens.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 16:09:51 by sguillot          #+#    #+#             */
/*   Updated: 2024/07/11 16:57:43 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static bool ft_set_listen_param(t_data *data, std::vector<std::string> tokens)
{
    (void)data;
    (void)tokens;
    return (true);
}

static bool ft_set_host_name_param(t_data *data, std::vector<std::string> tokens)
{
    (void)data;
    (void)tokens;
    return (true);
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
    std::string param_array[6] = {"listen", "host_name", "server_name", "error_page",
        "client_max_body", "location"};
    
    typedef bool (*param_function)(t_data *, const std::vector<std::string>&);

    param_function param_functions[] = {
        &set_listen_param,
        &set_host_name_param,
        &set_server_name_param,
        &set_error_page_param,
        &set_client_max_body_param,
        &set_location_param
    };
    
    for (int i = 0; i < param_array.size(); i++)
    {
        if (tokens[0] == param_array[i])
        {
            switch(i)
            {
                case 0:
                    return (param_function[0](data, tokens));
                case 1:
                    return (param_function[1](data, tokens));
                case 2:
                    return (param_function[2](data, tokens));
                case 3:
                    return (param_function[3](data, tokens));
                case 4:
                    return (param_function[4](data, tokens));
                case 5:
                    return (param_function[5](data, tokens));
                default:
                    break;
            }
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
    std::string test;
    for (int i = 0; (long unsigned int)i < tokens.size(); i++)
    {
        test += tokens[i];
        test += "|";
    }
    std::cout << test << std::endl;
    
    if (!tokens.empty())
    {
        if (data->parsing.parsing_ended == true)
        {
            std::string error_message = "invalid line (no lines allowed after server configurations) [./webserv --help]: ";
            for (int i = 0; (long unsigned int)i < tokens.size(); i++)
            {
                error_message += tokens[i];
                error_message += " ";
            }
            return (ft_error(data, error_message));
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
