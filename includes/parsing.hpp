/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 14:30:30 by sguillot          #+#    #+#             */
/*   Updated: 2024/07/10 14:46:38 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
# define PARSING_HPP

#include "struct.hpp"

/* parsing_g.cpp */
bool    parsing_g(t_data *data, int argc, char **argv);

/* serverConfiguration.cpp */
bool    ft_serverConfiguration(t_data *data, const std::string &line);

#endif