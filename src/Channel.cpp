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

	options_types[0] = "i";
	options_types[1] = "k";
	options_types[2] = "l";
	options_types[3] = "o";
	options_types[4] = "t";

	options_ptr[0] = &Channel::i_request;
	options_ptr[1] = &Channel::k_request;
	options_ptr[2] = &Channel::l_request;
	options_ptr[3] = &Channel::o_request;
	options_ptr[4] = &Channel::t_request;
	
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
void Channel::i_request(User *user, std::vector<std::string> line,int *param,std::string *validparam,std::string *validoptions)
{
	(void)user;
	(void)validparam;
	(void)param;
	if(line[2][0] == '-')
	{
		*validoptions += "i";
		this->remove_mod(user,1);
		std::cout << "Removing invite only restriction" << std::endl;
	}
	else if(line[2][0] == '+')
	{
		*validoptions += "i";
		this->set_mod(user,1);
		std::cout << "Set invite only restriction" << std::endl;
	}
}
void Channel::o_request(User *user, std::vector<std::string> line,int *param,std::string *validparam,std::string *validoptions)
{
	if(line[2][0] == '-')
	{
		if(line[*param].c_str() && !this->take_privilege(user ,line[*param]))
		{
			*validoptions += "o";
			*validparam += line[*param] + " ";

			std::cout << "Removing user privilege" << std::endl;
			*param = *param + 1 ;
		}
		else
		{
			//impossible to remove privilege
		} 
						
	}
	else if(line[2][0] == '+')
	{
		if(line[*param].c_str())
		{
			if(!this->give_privilege(user,line[*param]))
			{
				*validoptions += "o";
				*validparam += line[*param] + " ";
				*param = *param + 1 ;
				std::cout << "Set privilege to a user" << std::endl;
			}
		}
	}				
}
void Channel::l_request(User *user, std::vector<std::string> line,int *param,std::string *validparam,std::string *validoptions)
{
	if(line[2][0] == '-')
	{
		if(!this->remove_mod(user,4))
		{
			*validoptions += "l";
			std::cout << "Removing user limit restriction" << std::endl;
		}
		else
		{
			//user n'a pas les droits ou le mod est deja off
		}
	}
	else if(line[2][0] == '+')
	{
		if(line[*param].c_str())
		{
			try
			{
				if(!this->set_mod(user,4))
				{
					int nb = std::atoi(line[*param].c_str());
					std::cout << nb << "\n";
					this->limit_user = nb ;
					*validparam += line[*param] + " ";
					*param = *param + 1 ;
					std::cout << "Set user limit restriction" << std::endl;
					*validoptions += "l";
				}
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
		}
	}
}
void Channel::k_request(User *user, std::vector<std::string> line,int *param,std::string *validparam,std::string *validoptions)
{
	if(line[2][0] == '-')
	{
		if(!this->remove_mod(user,2))
		{
			*validoptions += "k";
			this->password = "";
			std::cout << "Removing password restriction" << std::endl;
		}
		else
		{
			//user a pas les droits ou le mod est deja off
		}
	}
	else if(line[2][0] == '+')
	{
		if(line[*param].c_str())
		{
			if(!this->set_mod(user,2))
			{
				*validoptions += "k";
				this->password = line[*param];
				*validparam += line[*param] + " ";
				*param = *param + 1 ;
				std::cout << "Set password restriction" << std::endl;
			}
		}
	}
}
void Channel::t_request(User *user, std::vector<std::string> line,int *param,std::string *validparam,std::string *validoptions)
{
	(void)param;
	(void)validparam;
	if(line[2][0] == '-')
	{
		if(!this->remove_mod(user,5))
		{
			*validoptions += "t";
			std::cout << "Removing topic restriction" << std::endl;
		}
	}
	else if(line[2][0] == '+')
	{
		if(!this->set_mod(user,5))
		{
			*validoptions += "t";
			std::cout << "Setting topic restriction" << std::endl;
		}
	}

}
		
void Channel::update_mod(User *user, std::vector<std::string> line)
{
	/* ERR_NEEDMOREPARAMS              ERR_KEYSET
    	ERR_CHANOPRIVSNEEDED
    	ERR_USERNOTINCHANNEL           ERR_UNKNOWNMODE*/
	std::string options;
	int param = -1;
	std::string response;
	std::string validoptions;
	std::string validparam;

	validparam = "";
	validoptions = "";
	response = "";
	if(line.size() < 3 || !IsValidChannelName(line[1]))
		return;
	if(!IsMod(user))
	{
		std::cout << user->get_nickname() << " is not mod of " << line[1] << std::endl;
		// USER IS NOT MOD ERR_NOCHANMODES :ircserv.42.fr 482 lloisel_ #bob :You're not channel operator\r\n
		std::cout << " Sending response to a mode command" << std::endl;
		response = ":ircserv.42.fr " ;
		response += ERR_CHANOPRIVSNEEDED;
		response += " " + user->get_nickname()+ " "+ line[1] +" :You're not channel operator\r\n";
		std::cout << response << std::endl; 
		send(user->get_socket(),response.c_str(),response.length(),0);
		return;
	}
	
	if(!IsValidChannelName(line[1]))
		std::cout << "Channel name is not valid" << std::endl;
	options = line[2];
	if (line.size() >= 4)
		param = 3;
	if(options.size() >= 2)
	{
		std::string::iterator current = options.begin();
		std::cout << options << std::endl;

		if(options[0] == '-' || options[0] == '+')
		{
			validoptions += options[0];
			current++;
			while (current != options.end())
			{
				int i = 0;
				while(i < 5)
				{
					if(*(options_types[i].c_str()) == *current)
					{	
						std::cout << "We found a valid option :" << options_types[i]<< std::endl;
						(this->*options_ptr[i])( user, line, &param, &validparam, &validoptions);
						break;
					}std::cout << " Sending response to a mode command" << std::endl;
			response = ":" + user->get_nickname() + "!" + user->get_nickname() + "@localhost";
			response += " MODE " + line[1] + " " + validoptions + " " + validparam;
			response += "\r\n";
			std::cout << response << std::endl; 
					i++;
				}
				if(i == 5)
				{
					//invalid option
				}
				current ++;
			}
			
		}
		else
		{
			// pas de plus ou de moins 
		}
		if(validoptions.size() >= 1)
		{
			std::cout << " Sending response to a mode command" << std::endl;
			response = ":" + user->get_nickname() + "!" + user->get_nickname() + "@localhost";
			response += " MODE " + line[1] + " " + validoptions + " " + validparam;
			response += "\r\n";
			std::cout << response << std::endl; 
			send_all(response);

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
