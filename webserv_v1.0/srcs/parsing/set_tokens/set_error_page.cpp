/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_error_page.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:21:47 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/12 16:12:51 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::ft_set_error_page_param(std::vector<std::string> tokens)
{
    if (tokens.size() != 3)
    {
        return (ft_invalid_line(tokens));
    }

    std::stringstream ss(tokens[1]);
    int code = 0;
    ss >> code;

    std::string error_page = tokens[2];

    if (tokens[1].empty())
    {
        error("Error: invalid argument (error page): " + tokens[1]);
        return;
    }

    if (error_page.empty() && error_page[error_page.length() - 1] == ';')
    {
        error_page.erase(error_page.length() - 1, 1);
    }
    else
    {
        error("Error: invalid argument (error page): " + tokens[2]);
        return;
    }

	this->currentConfig->setErrorPage(code, error_page);
}
