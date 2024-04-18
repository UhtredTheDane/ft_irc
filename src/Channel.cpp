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
	this->theme = "";
	this->name = name;
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

void Channel::update_mod(User *user, std::vector<std::string> line)
{
	/* ERR_NEEDMOREPARAMS              ERR_KEYSET
           ERR_NOCHANMODES                 ERR_CHANOPRIVSNEEDED
           ERR_USERNOTINCHANNEL            ERR_UNKNOWNMODE*/
	std::string options;
	int param = -1;
	std::string response;
	std::string validoptions;
	std::string validparam;

	validparam = "";
	validoptions = "";
	response = "";
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
			validoptions += "-";
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
							validoptions += "i";
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
							validoptions += "k";
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
							validoptions += "o";
							validparam += line[param] + " ";

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
							validoptions += "l";
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
							validoptions += "t";
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
			validoptions += "+";
			while (current != options.end())
			{
				
				std::cout << compare.find_first_of('i') << std::endl;
				if(compare.find(*current))
				{
					
					switch (*current)
					{
					case 'i':
						if(!this->set_mod(user,1))
						{
							validoptions += "i";
							std::cout << "Set invite only restriction" << std::endl;
						}
						break;
					case 'k':
						if(line[param].c_str())
						{
							if(!this->set_mod(user,2))
							{
								validoptions += "k";
								this->password = line[param];
								validparam += line[param] + " ";
								param ++;
								std::cout << "Set password restriction" << std::endl;
							}
						}
						break;
					case 'o':
						if(line[param].c_str())
						{
							if(!this->give_privilege(user,line[param]))
							{
								validoptions += "o";
								validparam += line[param] + " ";
								param ++;
								std::cout << "Set privilege to a user" << std::endl;
							}
						}
						break;
					case 'l':
						if(line[param].c_str())
						{
							try
							{
								if(!this->set_mod(user,4))
								{
									int nb = std::atoi(line[param].c_str());
									std::cout << nb << "\n";
									this->limit_user = nb ;
									validparam += line[param] + " ";
									param ++;
									std::cout << "Set user limit restriction" << std::endl;
									validoptions += "l";
								}
							}
							catch(const std::exception& e)
							{
								std::cerr << e.what() << '\n';
							}
						}
						break;
					case 't':
						
						if(!this->set_mod(user,5))
						{
							validoptions += "t";
							std::cout << "Setting topic restriction" << std::endl;
						}
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
		if(validoptions.size() > 1)
		{
			std::cout << " Sending response to a mode command" << std::endl;
			response = ":" + user->get_nickname() + "!" + user->get_nickname() + "@localhost";
			response += " MODE " + line[1] + " " + validoptions + " " + validparam;
			response += "\r\n";
			std::cout << response << std::endl; 
			send(user->get_socket(), response.c_str(), response.length(), 0);

		}
		else
			std::cout << " No valid options " << std::endl;
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
	it = std::find(users.begin(),admin_users.end(),user);
	if(it != users.end())
		return 1;
	return 0;
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
