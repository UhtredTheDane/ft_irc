/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agengemb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 16:07:38 by agengemb          #+#    #+#             */
/*   Updated: 2024/04/19 22:40:28 by agengemb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/Server_handler.hpp"
#include "../include/reply_macros.hpp"
#include "../include/reply_macros_error.hpp"

Server_handler::Server_handler(Server* serv)
{
	this->serv = serv;
	msg = Server_msg();
	request_types[0] = "CAP";
	request_types[1] = "PASS";
	request_types[2] = "NICK";
	request_types[3] = "USER";
	request_types[4] = "PING";
	request_types[5] = "JOIN";
	request_types[6] = "MODE";
	request_types[7] = "KICK"; 
	request_types[8] = "PRIVMSG";
	request_types[9] = "PART";
	request_types[10] = "INVITE";

	requests_ptr[0] = &Server_handler::capls_request;
	requests_ptr[1] = &Server_handler::pass_request;
	requests_ptr[2] = &Server_handler::nick_request;
	requests_ptr[3] = &Server_handler::user_request;
	requests_ptr[4] = &Server_handler::pong_request;
	requests_ptr[5] = &Server_handler::join_request;
	requests_ptr[6] = &Server_handler::mode_request;
	requests_ptr[7] = &Server_handler::kick_request;
	requests_ptr[8] = &Server_handler::privmsg_request;
	requests_ptr[9] = &Server_handler::part_request;
	requests_ptr[10] = &Server_handler::invite_request;
}

Server_msg* Server_handler::get_msg(void)
{
	return (&msg);
}
void Server_handler::capls_request(User* user)
{
	if (user->get_isRegistered() == 0)
	{
		user->set_isRegistered(1);
	}
}

void Server_handler::pass_request(User* user)
{
	if((!serv->check_password(split_line[1])) || (!user->get_isRegistered()) == 1)
	{
		user->set_isRegistered(0);
		throw (Server_handler::Err_PasswordIncorrect());
	}
	user->set_isPasswordValid(true);
}

void Server_handler::nick_request(User* user)
{
	if(user->get_isPasswordValid() && user->get_isRegistered() == 1)
	{
		while (serv->is_on_serv(split_line[1]))
		{
			msg.nicknameinuse_msg(user, split_line[1], split_line[1] + "_");
			split_line[1] += "_";
		}
		user->set_nickname(split_line[1]);
	}
	else
	{
		throw(Server_handler::Err_NotRegistred(user->get_socket()));
	}
}

void Server_handler::invite_request(User* user)
{
	User *target;
	Channel *chan;
	std::string reply;

	std::cout << "Handling an invite request" << std::endl;
	try
	{
		if(this->split_line.size() != 3)
			msg.needmoreparams_msg(user,this->raw_msg);
		else
		{
			chan = this->serv->get_channels().at(this->split_line[2]);//channel exist 
			
				target = chan->findUserByName(*chan->get_users(),this->split_line[1]) ;//user in the channel
				if(!target)
				{
					target = this->serv->findUserByName(this->split_line[1]);//user exist
					if(target)
					{
						std::cout << "sending invite " << std::endl; 	
						reply = ":" + user->get_nickname() + "!" + "" + " INVITE " + target->get_nickname() + " "+ chan->get_name() + "\r\n";
						chan->invite_user(target);
						send(target->get_socket(),reply.c_str(),reply.length(),0);
					}
					else
					{
						std::cout << "target not connected to the server" << std::endl;
						throw(Server_handler::Err_NotOnChannel(this->split_line[1]));
					}	
				}
				else
				{
					std::cout << "user already in the channel " << std::endl;
					throw(Server_handler::Err_useronchannel(this->split_line[1],this->split_line[2]));
				}
			
		}
		
	}
	catch(const std::exception& e)
	{
		msg.nosuchchannel_msg(user,split_line[2]);
	}

}

void Server_handler::user_request(User* user)
{
	if(split_line.size() < 4)
			throw(Server_handler::Err_NeedMoreParams("USER"));
	if(!user->get_nickname().empty() && user->get_isPasswordValid() && user->get_isRegistered() == 1)
	{
		user->set_username(split_line[1]);
		user->set_hostname(split_line[2]);
		user->set_servername(split_line[3]);
		user->set_realname(split_line[3]);
		user->set_identifier();
		msg.welcome_msg(user);
		msg.yourhost_msg(user);
		msg.created_msg(user);
		msg.myinfo_msg(user);
		user->set_isRegistered(2);
	}
	else
	{
		std::cout << "user\n";
		throw(Server_handler::Err_NotRegistred(user->get_socket()));
	}

}

void Server_handler::pong_request(User* user)
{
	msg.pong_msg(user);
}

void Server_handler::join_request(User* user)
{
	if (split_line.size() < 2)
	{
		throw(Server_handler::Err_NeedMoreParams(split_line[0]));
	}
	std::stringstream list_name(split_line[1]);
	std::string channel_name;
	Channel* current_chan;
	while (getline(list_name, channel_name, ','))
	{
		try
		{	
			current_chan = serv->get_channels().at(channel_name);
		}
		catch (std::out_of_range& oor)
		{
			msg.join_msg(user, serv->add_channel(channel_name, user));
			return ;
		}
		if (current_chan->IsOption(1))
		{
			throw(Err_InviteOnlyChan(channel_name));
		}
		else if (current_chan->IsOption(2) && !current_chan->check_key(split_line[2]))
		{
			throw(Err_BadChannelKey(channel_name));
		}
		else if (current_chan->IsOption(4) && current_chan->is_full())
		{
			throw(Err_ChannelIsFull(channel_name));
		}
		current_chan->add_user(user);
		msg.join_msg(user, current_chan);
	}
}

void Server_handler::mode_request(User* user)
{
	try
	{
		Channel* target_chan = serv->get_channels().at(split_line[1]);
		target_chan->update_mod(user,split_line);
	}
	catch (std::out_of_range& oor)
	{
	}
}

void Server_handler::kick_request(User* user)
{
	int i = 0;
	try
	{
		if(split_line.size() < 3)
			throw(Server_handler::Err_NeedMoreParams(split_line[0]));
		Channel *curent_chan = serv->get_channels().at(split_line[1]);
	for (std::vector<User*>::iterator it = curent_chan->get_admins()->begin(); it != curent_chan->get_admins()->end();)
	{
		if(user == *it)
		{
		i = 1;
		}
		it++;
	}
	if(i == 0)
		throw(Server_handler::Err_chanoprivsneeded(curent_chan->get_name()));
	i = 0;
	for (std::vector<User*>::iterator it = curent_chan->get_users()->begin(); it != curent_chan->get_users()->end();)
	{		
		if(user == *it)
		{
			i = 1;
		}
		it++;
	}
	if(i == 0)
		throw(Server_handler::Err_NotOnChannel(this->split_line[1]));/*ERR_NOTONCHANNEL*/;
	if(msg.kick_msg(user, curent_chan,split_line) == -1)
			throw(Server_handler::Err_UserNotInChannel(split_line[2], split_line[1]))/*ERR_USERNOTINCHANNEL */;
	}
	catch(std::out_of_range& oor)
	{
				throw(Server_handler::Err_NoSuchChannel(split_line[1]));
	}

}

void Server_handler::privmsg_request(User* user)
{
	try
	{
		if(split_line.size() < 3)
			throw(Server_handler::Err_NeedMoreParams(split_line[0]));
		if(split_line[1][0] == '#')
		{
			Channel *curent_chan = serv->get_channels().at(split_line[1]);	
			std::map<int, User*> users_map = serv->get_users();
			Message c_msg(split_line[2], user);
			curent_chan->add_message(&c_msg);
			if(msg.chan_msg(user, curent_chan, split_line) == -1)
			{
				throw(Server_handler::Err_CannotSendToChan(split_line[1]));
			}
		}
		else
		{
			std::map<int, User*> users_map = serv->get_users();
			if(msg.priv_msg(user, split_line, users_map) == -1)
			{
				throw(Server_handler::Err_NoSuchNick(split_line[1]));
			}
		}
	}
	catch (std::out_of_range& oor)
	{
		throw(Server_handler::Err_CannotSendToChan(split_line[1]));
	}
}

void Server_handler::part_request(User* user)
{
		try
		{
			Channel *current_chan = serv->get_channels().at(split_line[1]);
			msg.leave_msg(user, current_chan);
			current_chan->delete_user(user);
		}
		catch (std::out_of_range& oor)
		{
			throw (Server_handler::Err_NoSuchChannel(split_line[1]));
		}
}

void Server_handler::processing_request(User* user, std::string& request)
{
	std::stringstream coco(request);
	std::string word;

	while (getline(coco, word, ' '))
		split_line.push_back(word);
	for (int i = 0; i < 11; ++i)
	{
		 if(i > 3 && user->get_isRegistered() != 2)
		 {
		 	i = 0;
			split_line.clear();
			throw(Server_handler::Err_NotRegistred(user->get_socket()));
		 }
		else if (!split_line[0].compare(request_types[i]))
		{
			try
			{
				(this->*requests_ptr[i])(user);
			}
			catch (ExceptionInterface& e)
			{
				e.handle(user, &msg);
			}
			break;
		}
	}
	split_line.clear();
}

void Server_handler::request_handler(int client_socket, std::string& request)
{
	User* user;
	std::string delimiter = "\r\n";
	std::string token;
	size_t delim_pos;

	std::map<int, User*> users_map = serv->get_users();
	try
	{
		user = users_map.at(client_socket);
		if (request.find(delimiter, 0) != std::string::npos)
		{
			user->buffer += request;
			while((delim_pos = user->buffer.find(delimiter)) != std::string::npos)
			{
				token = user->buffer.substr(0, delim_pos);
				std::cout << "\033[34m" << "[RECEIVED]" << token << "\033[0m" << std::endl;
				this->raw_msg = token;
				try
				{
				processing_request(user, token);
				}
				catch (Server_handler::Err_NotRegistred& e)
				{
					std::cout << "buffer deleted\n";
					user->set_isRegistered(0);
					get_msg()->notregistred_msg(client_socket);
				}
				user->buffer.erase(0, delim_pos + delimiter.length());
			}
		}
		else
		{
			user->buffer += request;
		}
	}
	catch(std::out_of_range& oor)
	{
	}

}

User *Server_handler::findUserByName(std::vector<User *> v,std::string name)
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

Server_handler::Err_NeedMoreParams::Err_NeedMoreParams(std::string channel) : channel(channel)
{
	
}

std::string Server_handler::Err_NeedMoreParams::get_channel(void)
{
	return (channel);
}

Server_handler::Err_InviteOnlyChan::Err_InviteOnlyChan(std::string channel) : channel(channel)
{
	
}

std::string Server_handler::Err_InviteOnlyChan::get_channel(void)
{
	return (channel);
}

Server_handler::Err_ChannelIsFull::Err_ChannelIsFull(std::string channel) : channel(channel)
{
	
}

std::string Server_handler::Err_ChannelIsFull::get_channel(void)
{
	return (channel);
}

Server_handler::Err_BadChannelKey::Err_BadChannelKey(std::string channel) : channel(channel)
{
	
}

std::string Server_handler::Err_BadChannelKey::get_channel(void)
{
	return (channel);
}

Server_handler::Err_NoSuchChannel::Err_NoSuchChannel(std::string str) : str(str)
{
	
}

std::string Server_handler::Err_NoSuchChannel::get_channel(void)
{
	return (str);
}

Server_handler::Err_CannotSendToChan::Err_CannotSendToChan(std::string str) : str(str)
{
	
}

std::string Server_handler::Err_CannotSendToChan::get_channel(void)
{
	return (str);
}

Server_handler::Err_NoSuchNick::Err_NoSuchNick(std::string str) : str(str)
{
	
}

std::string Server_handler::Err_NoSuchNick::get_channel(void)
{
	return (str);
}

Server_handler::Err_NotOnChannel::Err_NotOnChannel(std::string str) : str(str)
{
	
}

Server_handler::Err_useronchannel::Err_useronchannel(std::string nick, std::string channel) : channel(channel), nick(nick)
{

}	

std::string Server_handler::Err_useronchannel::getNick()
{
	return (nick);
}

std::string Server_handler::Err_useronchannel::get_channel()
{
	return (channel);
}

Server_handler::Err_UserNotInChannel::Err_UserNotInChannel(std::string nick, std::string channel) : channel(channel), nick(nick)
{

}	

std::string Server_handler::Err_UserNotInChannel::getNick()
{
	return (nick);
}

std::string Server_handler::Err_UserNotInChannel::get_channel()
{
	return (channel);
}

std::string Server_handler::Err_NotOnChannel::get_channel(void)
{
	return (str);
}
Server_handler::Err_chanoprivsneeded::Err_chanoprivsneeded(std::string channel) : channel(channel)
{
	
}

std::string Server_handler::Err_chanoprivsneeded::get_channel(void)
{
	return (channel);
}


Server_handler::Err_NotRegistred::Err_NotRegistred(int socket) : socket(socket)
{

}	

int Server_handler::Err_NotRegistred::get_socket()
{
	return (socket);
}