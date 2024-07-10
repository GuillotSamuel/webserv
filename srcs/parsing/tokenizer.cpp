/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:36:24 by sguillot          #+#    #+#             */
/*   Updated: 2024/07/10 17:11:29 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

bool    ft_tokenizer(t_data *data, const std::string &line)
{
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

    ft_set_tokens(data, tokens);

    return (true);
}

