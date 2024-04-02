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
int Channel::remove_mod(User *user, int modif)
{
	std::vector<User *>::iterator it;
	it = std::find(admin_users.begin(),admin_users.end(),user);
	if(it != admin_users.end())
	{
		if(mask & (1 << modif))
		{
			mask = mask - (1 << modif);
			return 0;
		}
	}
	else
	{
		//gestion le user existe pas ou n'est pas mod
	}
	return 1;
}
int Channel::set_mod(User *user, int modif)
{
	std::vector<User *>::iterator it;
	it = std::find(admin_users.begin(),admin_users.end(),user);
	if(it != admin_users.end())
	{
		if(!(mask & (1 << modif)))
		{
			mask = mask + (1 << modif);
			return 0;
		}
		else
			return 1;
	}
	else
	{
		//gestion le user existe pas ou n'est pas mod
		return 1;
	}
}
std::string Channel::getName()
{
	return this->name;
}

void Channel::update_mod(User *user, std::vector<std::string> line)
{
	
	std::string options;
	int param = -1;
	
	std::cout << "Updating mode of a channel" << std::endl;
	std::string compare = " ikolt ";
	if(line.size() >= 3) 
		options = line[2];
	if(options.size() >= 2)
	{
		if (line.size() >= 4)
		{
			param = 3;
		}
		std::string::iterator current = options.begin();
		std::cout << options << std::endl;

		if(options[0] == '-')
		{
			current++;
			while (current != options.end())
			{
				std::cout << *current << std::endl;
				if(compare.find(*current))
				{
					switch (*current)
					{
					case 'i':
						if(!this->remove_mod(user,1))
						{
							std::cout << "Removing invite only restriction" << std::endl;
						}	
						else 
						{
							//user a pas les droits ou le mod est deja off
						}
						break;
					case 'k':
						
						if(!this->remove_mod(user,2))
						{
							this->password = "";
							std::cout << "Removing password restriction" << std::endl;
						}
						else
						{
							//user a pas les droits ou le mod est deja off
						}
						
						break;
					case 'o':
						if(line[param].c_str() && !this->take_privilege(user ,line[param]))
						{
							std::cout << "Removing user privilege" << std::endl;
							param++;
						}
						else
						{
							//impossible to remove privilege
						} 
						break;
					case 'l':
						if(!this->remove_mod(user,4))
						{
							std::cout << "Removing user limit restriction" << std::endl;
						}
						else
						{
							//user n'a pas les droits ou le mod est deja off
						}
						break;
					case 't':
						
						if(!this->remove_mod(user,5))
						{
							std::cout << "Removing topic restriction" << std::endl;
						}
						break;
					}
				}
				current ++;
			}
			
		}
		else if(options[0] == '+')
		{
			current++;
			while (current != options.end())
			{
				
				std::cout << compare.find_first_of('i') << std::endl;
				if(compare.find(*current))
				{
					
					switch (*current)
					{
					case 'i':
						this->set_mod(user,1);
						std::cout << "Set invite only restriction" << std::endl;
						break;
					case 'k':
						if(line[param].c_str())
						{
							this->password = line[param];
							this->set_mod(user,2);
							param ++;
							std::cout << "Set password restriction" << std::endl;
						}
						break;
					case 'o':
						if(line[param].c_str())
						{
							this->give_privilege(user,line[param]);
							param ++;
							std::cout << "Set privilege to a user" << std::endl;
						}
						break;
					case 'l':
						if(line[param].c_str())
						{
							this->limit_user = std::atoi(line[param].c_str());
							this->set_mod(user,4);
							param ++;
							std::cout << "Set user limit restriction" << std::endl;
						}
						break;
					case 't':
						this->set_mod(user,5);
						std::cout << "Removing topic restriction" << std::endl;
						break;
					}
				}
				current ++;
			}
		}
		else
		{
			// pas de plus ou de moins 
		}
	}
}

int  Channel::give_privilege(User *user,std::string name)
{
	User *u;

	u = findUserByName(users,name);
	std::cout << "On essaye de donner des droits a : "<< name << '"' << std::endl;
	if(!u)
		return(1);
	if(findUserByName(admin_users,name))
	{
		std::cout << user->get_nickname() << " is already mod" << std::endl;
		return 1;
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
int Channel::IsMod(User *user)
{
	std::vector<User *>::iterator it;
	it = std::find(admin_users.begin(),admin_users.end(),user);
	if(it != admin_users.end())
		return 1;
	return 0;
}