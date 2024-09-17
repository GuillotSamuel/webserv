/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_auto_index.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:48:12 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/17 12:53:48 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::ft_set_auto_index(std::vector<std::string> tokens)
{
	if (tokens.size() != 2 || (tokens[1] != "on" && tokens[1] != "off"))
	{
		ft_invalid_line(tokens);
	}

	this->currentConfig->setAutoIndex(tokens[1]);
}
