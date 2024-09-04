/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:44:33 by sguillot          #+#    #+#             */
/*   Updated: 2024/07/10 14:43:39 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

bool    ft_error(t_data *data, std::string str)
{
    std::cerr << "Error : " << str << std::endl;
    ft_free_all(data);
    return (false);
}
