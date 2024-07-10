/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:36:24 by sguillot          #+#    #+#             */
/*   Updated: 2024/07/10 15:01:17 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

bool    ft_tokenizer(t_data *data, const std::string &line)
{
    (void)data; // TO REMOVE

    std::vector<std::string>    tokens;
    std::string                 buffer;
    std::string::const_iterator line_copy;
    
    for (line_copy = line.begin(); line_copy != line.end(); ++line_copy)
    {
        if (!std::isspace(*line_copy))
        {
            buffer += *line_copy;
        }
        else if (!buffer.empty())
        {
            tokens.push_back(buffer);
            buffer.clear();
        }
    }
    
    if (!buffer.empty())
    {
        tokens.push_back(buffer);
    }

    for (std::vector<std::string>::const_iterator it = tokens.begin(); it != tokens.end(); ++it)
    {
        const std::string &token = *it;

        std::cout << token << " ";
    }
    std::cout << std::endl;

    return (true);
}

