/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agengemb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:14:33 by agengemb          #+#    #+#             */
/*   Updated: 2024/03/28 01:31:48 by agengemb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"

Channel::Channel(std::string& name, User* user_admin)
{
	this->name = name;
	this->admin_users.push_back(user_admin);
	users.push_back(user_admin);
}

Channel::~Channel(void)
{

}

void Channel::add_user(User *user)
{
	users.push_back(user);
}

void Channel::delete_user(User* to_delete)
{
	for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (to_delete->get_socket() == (*it)->get_socket())
		{
			users.erase(it);
			break;
		}
	}
}

void Channel::add_message(Message* msg)
{
	msgs.push_back(msg);
}

std::string Channel::get_theme(void)
{
	return(this->theme);
}

std::string Channel::get_name(void)
{
	return(this->name);
}

std::vector<User*>* Channel::get_users(void)
{
	return (&users);
}

std::vector<User*>* Channel::get_admins(void)
{
	return (&admin_users);
}