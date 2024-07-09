/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:44:33 by sguillot          #+#    #+#             */
/*   Updated: 2024/07/09 15:30:33 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void    ft_error(t_data *data, std::string str)
{
    std::cerr << "Error : " << str << std::endl;
    ft_free_all(data);
    exit(EXIT_FAILURE);
}
