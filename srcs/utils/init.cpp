/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 15:45:21 by sguillot          #+#    #+#             */
/*   Updated: 2024/07/12 15:48:24 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static void    ft_init_parsing(t_data *data)
{
    data->parsing.fd = -1;
    data->parsing.parsing_started_server = false;
    data->parsing.parsing_started_brace = false;
    data->parsing.parsing_ended = false;
}

void    ft_init(t_data *data, ServerConfiguration *server_configuration)
{
    data->p_server_configuration = server_configuration;
    ft_init_parsing(data);
}
