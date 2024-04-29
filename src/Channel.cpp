/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agengemb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:14:33 by agengemb          #+#    #+#             */
/*   Updated: 2024/04/19 22:39:20 by agengemb         ###   ########.fr       */
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
	std::cout << "password: " << password << " et key: " << key << std::endl;
	bool res;
	res = key.compare(password) == 0;
	std::cout << "res: " << res << std::endl;
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

	if(name.c_str())
		u = findUserByName(users,name);
	else
		return (1);
	std::cout << "On essaye de donner des droits a : "<< name << '"' << std::endl;
	if(!u)
		return (1);
	if(findUserByName(admin_users,name))
	{
		std::cout << user->get_nickname() << " is already mod" << std::endl;
		throw(AlreadyMod());
	}
	else
	{
		std::cout << "giving privilege to " << u->get_nickname() << std::endl;
		this->admin_users.push_back(u);
	}
	for(std::vector<User *>::iterator it  = admin_users.begin(); it != admin_users.end();it ++)
	{
			std::cout << (*it)->get_nickname() << std::endl;
	}
	return 0;
}

int Channel::take_privilege(User *user,std::string name)
{
	User *u;

	u = findUserByName(admin_users,name);
	if(admin_users.size() > 1)
	{
		for(std::vector<User *>::iterator it  = admin_users.begin(); it != admin_users.end();it ++)
		{
			if(*it == u)
			{
				std::cout << "taking privilege to " << user->get_nickname() << std::endl;
				it = admin_users.erase(it);
				return 0;
			}
		}
		std::cout << u->get_nickname() << " is not a moderator of a channel" << std::endl;
		return 1;
	}
	else
	{
		std::cout << "You tried to erase the last admin of a channel" << std::endl;
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
		std::cout << "User already in the channel " << std::endl;
	}
}
void Channel::erase_invite(User* user)
{
	for(std::vector<User *>::iterator it  = invite.begin(); it != invite.end();it ++)
	{
			if((*it)->get_nickname() == user->get_nickname())
			{
				invite.erase(it);
			}
	}
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
