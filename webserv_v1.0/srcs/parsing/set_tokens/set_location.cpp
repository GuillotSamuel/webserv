/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_location.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:21:38 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/12 18:23:09 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::ft_set_uploads_location(std::vector<std::string> tokens)
{
    if (tokens.size() != 3 || tokens[2].empty())
    {
        ft_invalid_line(tokens);
    }

    this->currentConfig->setUploadsLocation(tokens[2]);
}

void Server::ft_set_errors_location(std::vector<std::string> tokens)
{
    if (tokens.size() != 3 || tokens[2].empty())
    {
        ft_invalid_line(tokens);
    }

    this->currentConfig->setErrorPagesLocation(tokens[2]);
}

void Server::ft_set_cgi_bin_location(std::vector<std::string> tokens)
{
    if (tokens.size() < 4 || tokens.size() % 2 == 1)
    {
        ft_invalid_line(tokens);
    }

    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].empty())
        {
            ft_invalid_line(tokens);
        }
    }
    
    this->currentConfig->setCgiBinLocation(tokens[1]);

    size_t i = 2;
    for (; (i + 1) < tokens.size(); i += 2)
    {
        this->currentConfig->setPathInfoCgi(tokens[i], tokens[i + 1]);
    }
}

void Server::ft_set_location_param(std::vector<std::string> tokens)
{
    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].empty())
        {
            ft_invalid_line(tokens);
        }
    }

    if (tokens[1] == "uploads")
    {
        ft_set_uploads_location(tokens);
    }
    else if (tokens[1] == "errors")
    {
        ft_set_errors_location(tokens);
    }
    else if (tokens[1] == "cgi-bin")
    {
        ft_set_cgi_bin_location(tokens);
    }
    else
    {
        ft_invalid_line(tokens);
    }
}