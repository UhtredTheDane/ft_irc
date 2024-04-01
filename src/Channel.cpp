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
void Channel::remove_mod(User *user, int modif)
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
void Channel::set_mod(User *user, int modif)
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
						this->remove_mod(user,1);
						std::cout << "Removing invite only restriction" << std::endl;
						break;
					case 'k':
						this->password = "";
						this->remove_mod(user,2);
						std::cout << "Removing password restriction" << std::endl;
						break;
					case 'o':
						this->take_privilege(user);
						std::cout << "Removing user privilege" << std::endl;
						break;
					case 'l':
						this->remove_mod(user,4);
						std::cout << "Removing user limit restriction" << std::endl;
						break;
					case 't':
						this->remove_mod(user,5);
						std::cout << "Removing topic restriction" << std::endl;
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
							this->give_privilege(user);
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

void Channel::give_privilege(User *user)
{
	std::cout << "giving privilege to " << user->get_nickname() << std::endl;
}

void Channel::take_privilege(User *user)
{
	std::cout << "taking privilege to " << user->get_nickname() << std::endl;
}