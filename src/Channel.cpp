/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agengemb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:14:33 by agengemb          #+#    #+#             */
/*   Updated: 2024/04/30 13:19:13 by agengemb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"
# include <iostream>

Channel::Channel(std::string& name, User* user_admin)
{
	this->theme = "";
	this->name = name;
	this->limit_user = 0;
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

bool Channel::is_full(void)
{
	if (static_cast<int>(users.size()) >= limit_user)
	{
		return (true);
	}
	return (false);
}

bool Channel::check_key(std::string& key)
{
	bool res;
	res = key.compare(password) == 0;
	return (res);
}

int Channel::remove_mod(User *user, int modif)
{
	(void)user;
	if(mask & (1 << modif))
	{
		mask = mask - (1 << modif);
		return 0;
	}
	return 1;
}
int Channel::set_mod(User *user, int modif)
{
	(void)user;
	if(!(mask & (1 << modif)))
	{
		mask = mask + (1 << modif);
		return 0;
	}
	return 1;
}

int  Channel::give_privilege(User *user,std::string name)
{
	User *u;

	(void)user;
	if(name.c_str())
		u = findUserByName(users,name);
	else
		return (1);
	if(!u)
		return (1);
	if(findUserByName(admin_users,name))
	{
		throw(AlreadyMod());
	}
	else
	{
		this->admin_users.push_back(u);
	}
	return 0;
}

int Channel::take_privilege(User *user,std::string name)
{
	User *u;

	(void)user;
	u = findUserByName(admin_users,name);
	if(admin_users.size() > 1)
	{
		for(std::vector<User *>::iterator it  = admin_users.begin(); it != admin_users.end();it ++)
		{
			if(*it == u)
			{
				it = admin_users.erase(it);
				return 0;
			}
		}
		return 1;
	}
	else
	{
		return 1;
	}
}
User *Channel::findUserByName(std::vector<User *> v,std::string name)
{
	for(std::vector<User *>::iterator it  = v.begin(); it != v.end();it ++)
	{
		if((*it)->get_nickname() == name)
		{
			return *it;
		}
	}
	return(NULL);
}

int Channel::IsOption(int option)
{
	return (mask & (1 << option));
}

int Channel::IsMod(User *user)
{
	std::vector<User *>::iterator it;
	it = std::find(admin_users.begin(),admin_users.end(),user);
	if(it != admin_users.end())
		return 1;
	return 0;
}

int Channel::IsInChannel(User *user)
{
	std::vector<User *>::iterator it;
	it = std::find(users.begin(),users.end(),user);
	if(it != users.end())
		return 1;
	return 0;
}
int Channel::IsValidChannelName(std::string name)
{
	return(name.size() > 2 && (name[0] == '#' || name[0] == '&'));
}

void Channel::invite_user(User *user)
{
	if(!findUserByName(invite,user->get_nickname()))
	{
		invite.push_back(user);
	}
	else
	{

	}
}

bool Channel::erase_invite(User* user)
{
	for (std::vector<User *>::iterator it  = invite.begin(); it != invite.end();it ++)
	{
		if ((*it)->get_nickname() == user->get_nickname())
		{
			invite.erase(it);
			return (true);
		}
	}
	return (false);
}
void Channel::set_topic(std::string str)
{
	this->theme = str;
}
void Channel::send_all(std::string str)
{
	std::vector<User*>::iterator it = this->users.begin();
	while(it != users.end())
	{
		send((*it)->get_socket(),str.c_str(),str.length(),0);
		it++;
	}
}
void Channel::set_limit_user(int const nb)
{
	this->limit_user = nb;
}
void Channel::set_password(std::string const str)
{
	this->password = str;
}
