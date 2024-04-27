/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AException.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agengemb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:10:23 by agengemb          #+#    #+#             */
/*   Updated: 2024/04/27 17:10:25 by agengemb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/AException.hpp"

int AException::get_socket(void)
{
    return (socket);
}

std::string AException::get_channel(void)
{
    return (channel);
}

std::string AException::get_nick(void)
{
    return (nick);
}

std::string AException::get_cmd(void)
{
    return (cmd);
}
