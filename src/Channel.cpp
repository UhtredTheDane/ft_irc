/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agengemb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:14:33 by agengemb          #+#    #+#             */
/*   Updated: 2024/03/21 08:45:08 by agengemb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"

Channel::Channel(std::string& theme, User* user_admin)
{
	this->theme = theme;
	this->name = theme;
	this->admin_users.push_back(user_admin);
	users.push_back(user_admin);
	this->mask = 0;

}
Channel::~Channel(void)
{

}

void Channel::add_user(User *user)
{
	users.push_back(user);
}

void Channel::add_message(Message* msg)
{
	msgs.push_back(msg);
}

std::string Channel::get_theme(void)
{
	return(this->theme);
}
std::vector<User*>* Channel::get_users(void)
{
	return (&users);
}
void Channel::remove_mod(User *user, Chanmod modif)
{
	std::vector<User *>::iterator it;
	it = std::find(admin_users.begin(),admin_users.end(),user);
	if(it != admin_users.end())
	{
		if(mask & (1 << modif))
		{
			mask = mask - (1 << modif);
		}
	}
	else
	{
		//gestion le user existe pas ou n'est pas mod
	}
}
void Channel::set_mod(User *user, Chanmod modif)
{
	std::vector<User *>::iterator it;
	it = std::find(admin_users.begin(),admin_users.end(),user);
	if(it != admin_users.end())
	{
		if(!(mask & (1 << modif)))
		{
			mask = mask + (1 << modif);
		} 
	}
	else
	{
		//gestion le user existe pas ou n'est pas mod
	}
}
std::string Channel::getName()
{
	return this->name;
}
void Channel::update_mod(User *user, std::vector<std::string> line)
{
	
}
