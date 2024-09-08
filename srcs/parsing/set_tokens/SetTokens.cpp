/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SetTokens.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 21:06:16 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/06 22:56:34 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::ft_set_tokens(std::vector<std::string> tokens)
{
	if (!tokens.empty())
	{
		if (this->parsing_ended == true)
		{
			ft_invalid_line(tokens);
		}
		else if ((this->parsing_started_server == false
			|| this->parsing_started_brace == false))
		{
			ft_start_set_tokens(tokens);
		}
		else if ((this->parsing_started_server == true
			&& this->parsing_started_brace == true)
			&& tokens.size() >= 1 && tokens[0] != "}")
		{
			ft_param_set_tokens(tokens);
		}
		else if ((this->parsing_started_server == true
			&& this->parsing_started_brace == true)
			&& tokens.size() >= 1 && tokens[0] == "}")
		{
			this->parsing_ended = true;
		}
	}
}

void	Server::ft_start_set_tokens(std::vector<std::string> tokens)
{
        if (tokens.size() == 1 && this->parsing_started_server == false && tokens[0] == "server")
        {
            this->parsing_started_server = true;
        }
        else if ((tokens.size() == 1 && this->parsing_started_server == false && tokens[0] == "server{")
            || (tokens.size() == 2 && this->parsing_started_server == false && tokens[0] == "server" && tokens[1] == "{"))
        {
            this->parsing_started_server = true;
            this->parsing_started_brace = true;
        }
        else if (tokens.size() == 1 && this->parsing_started_server == true && this->parsing_started_brace == false && tokens[0] == "{")
        {
            this->parsing_started_brace = true;
        }
        else
        {
            std::string error_message = "invalid line [./webserv --help]: ";
            for (int i = 0; (long unsigned int)i < tokens.size(); i++)
            {
                error_message += tokens[i];
                error_message += " ";
            }
            error(error_message);
        }
}

void	Server::ft_invalid_line(std::vector<std::string> tokens)
{
    std::string error_message = "invalid line (no lines allowed after server configurations) [./webserv --help]: ";
    for (int i = 0; (long unsigned int)i < tokens.size(); i++)
    {
        error_message += tokens[i];
        error_message += " ";
    }
    error(error_message);
}

void Server::ft_param_set_tokens(std::vector<std::string> tokens)
{
    const int nb_param = 6;
    
    const char* param_array[nb_param] = {"listen", "host_name", "server_name", "error_page",
                                         "client_max_body_size", "location"};
    void (Server::*param_functions[nb_param])(std::vector<std::string>) = {
        &Server::ft_set_listen_param,
        &Server::ft_set_host_name_param,
        &Server::ft_set_server_name_param,
        &Server::ft_set_error_page_param,
        &Server::ft_set_client_max_body_param,
        &Server::ft_set_location_param
    };

    for (int i = 0; i < nb_param; i++)
    {
        if (tokens.size() > 0 && std::strcmp(tokens[0].c_str(), param_array[i]) == 0)
        {
            (this->*param_functions[i])(tokens);
            return;
        }
    }

    std::string error_message = "unknown parameter: " + (tokens.size() > 0 ? tokens[0] : "");
    error(error_message);
}
