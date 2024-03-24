/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agengemb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:14:57 by agengemb          #+#    #+#             */
/*   Updated: 2024/03/21 00:42:00 by agengemb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Message.hpp"

Message::Message(std::string& msg, User *user)
{
	this->msg = msg;
	this->user = user;
}
std::string Message::get_msg(void)
{
	return(this->msg);
}