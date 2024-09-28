/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_location.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:21:38 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/27 14:30:57 by mmahfoud         ###   ########.fr       */
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
    if (tokens.size() < 3 || tokens.size() % 2 == 0)
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
    this->currentConfig->setCgiBinLocation(tokens[2]);

    size_t i = 3;
    for (; (i + 1) < tokens.size(); i += 2)
    {
        this->currentConfig->setPathInfoCgi(tokens[i], tokens[i + 1]);
    }
}

void Server::ft_set_location_param(std::vector<std::string> tokens)
{
    this->location_started = true;

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
    else if (tokens[1][0] == '/')
    {
        ft_location_pages(tokens);
    }
    else
    {
        ft_invalid_line(tokens);
    }
}
