/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agengemb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 16:07:38 by agengemb          #+#    #+#             */
/*   Updated: 2024/04/15 15:53:50 by agengemb         ###   ########.fr       */
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

void Server_handler::capls_request(User* user)
{
	if (user->get_isRegistered() == 0 && !split_line[1].compare("LS"))
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
}

void Server_handler::nick_request(User* user)
{
	if(user->get_isRegistered() == 1)
	{
		while (serv->is_on_serv(split_line[1]))
			split_line[1] += "_";
		user->set_nickname(split_line[1]);
	}
	// else if(user->get_isRegistered() == 2)
	// {
	// 	std::string c_msg;
	// 	while (serv->is_on_serv(split_line[1]))
	// 		split_line[1] += "_";
	// 	c_msg = ":" + user->get_identifier() + " NICK " + split_line[1] + "\r\n";
	// 	std::cout << "|" << c_msg << "|";
	// 	send(user->get_socket(), c_msg.c_str(), c_msg.length(), 0);
	// 	user->set_nickname(split_line[1]);
	else
	{
		throw(Server_handler::Err_AlreadyRegistred());
	}
}

void Server_handler::invite_request(User* user)
{
	User *target;
	Channel *chan;
	std::string reply;

     /*    
           ERR_NEEDMOREPARAMS              ERR_NOSUCHNICK
           ERR_NOTONCHANNEL                ERR_USERONCHANNEL
           ERR_CHANOPRIVSNEEDED*/
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
					}	
				}
				else
				{
					std::cout << "user already in the channel " << std::endl;
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
	if(user->get_isRegistered() == 1)
	{
		user->set_username(split_line[1]);
		user->set_hostname(split_line[2]);
		user->set_servername(split_line[3]);
		user->set_realname(split_line[3]);
		user->set_identifier();
		// user.show_userinfo(user);
		msg.welcome_msg(user);
		msg.yourhost_msg(user);
		msg.created_msg(user);
		msg.myinfo_msg(user);
		user->set_isRegistered(2);
	}
	else
	{
		throw(Server_handler::Err_AlreadyRegistred());
	}


}

void Server_handler::pong_request(User* user)
{
	msg.pong_msg(user);
}

void Server_handler::join_request(User* user)
{
	std::stringstream list_name(split_line[1]);
	std::string channel_name;
	while (getline(list_name, channel_name, ','))
	{
		try
		{	
			Channel* current_chan = serv->get_channels().at(channel_name);
			current_chan->add_user(user);
			msg.join_msg(user, current_chan);
		}
		catch (std::out_of_range& oor)
		{
			msg.join_msg(user, serv->add_channel(channel_name, user));
		}
	}
}

void Server_handler::mode_request(User* user)
{
	try
	{
		Channel* target_chan = serv->get_channels().at(split_line[1]);
		target_chan->update_mod(user->get_socket(),user,split_line);
	}
	catch (std::out_of_range& oor)
	{
	}
}

void Server_handler::kick_request(User* user)
{
	Channel *curent_chan = serv->get_channels().at(split_line[1]);
	for (std::vector<User*>::iterator it = curent_chan->get_admins()->begin(); it != curent_chan->get_admins()->end();)
	{
		if(user == *it)
		{
			std::string k_msg;
			for (std::vector<User*>::iterator it2 = curent_chan->get_users()->begin(); it2 != curent_chan->get_users()->end();)
			{
				if((*it2)->get_nickname() == split_line[2])
				{
					k_msg = ":" + user->get_identifier() + " KICK " + curent_chan->get_name() + " " + split_line[2] + " " + split_line[3] + "\r\n";
					send((*it2)->get_socket(), k_msg.c_str(), k_msg.length(), 0);
					break;
				}
				it2++;
			}
		}
		it++;
	}
}

void Server_handler::privmsg_request(User* user)
{
	try
	{
		if(split_line.size() < 3)
			throw(Server_handler::Err_NeedMoreParams());
		if(split_line[1][0] == '#')
		{
			Channel *curent_chan = serv->get_channels().at(split_line[1]);	
			std::map<int, User*> users_map = serv->get_users();
			Message c_msg(split_line[2], user);
			curent_chan->add_message(&c_msg);
			if(msg.chan_msg(user, curent_chan, split_line) == -1)
			{
				throw(Server_handler::Err_CannotSendToChan(split_line[1]));
			};
		}
		else
		{
			std::map<int, User*> users_map = serv->get_users();
			msg.priv_msg(user, split_line, users_map);	
			if(split_line[1].empty()||msg.priv_msg(user, split_line, users_map) == -1)
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
		if (!split_line[0].compare(request_types[i]))
		{
			try
			{
				(this->*requests_ptr[i])(user);
			}
			catch (Err_PasswordIncorrect& e)
			{
				msg.passwordincorrect_msg(user);
			}
			catch (Err_AlreadyRegistred& e)
			{
				msg.alreadyregistred_msg(user);
			}
			catch (Err_NoSuchChannel& e)
			{
				std::string strtest = e.get_str();
				msg.nosuchchannel_msg(user, strtest);
			}
			catch (Err_NotOnChannel& e)
			{
				std::string strtest = e.get_str();
				msg.notonchannel_msg(user, strtest);
			}
			catch (Err_CannotSendToChan& e)
			{
				std::string strtest = e.get_str();
				msg.cannotsendtochan_msg(user, strtest);;
			}
			catch (Err_NoSuchNick& e)
			{
				std::string strtest = e.get_str();
				msg.nosuchnick_msg(user, strtest);;
			}
			catch (Err_NeedMoreParams& e)
			{
				msg.needmoreparams_msg(user);;
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
				std::cout << token << std::endl;
				this->raw_msg = token;
				processing_request(user, token);
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

Server_handler::Err_NoSuchChannel::Err_NoSuchChannel(std::string str) : str(str)
{
	
}

std::string Server_handler::Err_NoSuchChannel::get_str(void)
{
	return (str);
}

Server_handler::Err_CannotSendToChan::Err_CannotSendToChan(std::string str) : str(str)
{
	
}

std::string Server_handler::Err_CannotSendToChan::get_str(void)
{
	return (str);
}

Server_handler::Err_NoSuchNick::Err_NoSuchNick(std::string str) : str(str)
{
	
}

std::string Server_handler::Err_NoSuchNick::get_str(void)
{
	return (str);
}


Server_handler::Err_NotOnChannel::Err_NotOnChannel(std::string str) : str(str)
{
	
}

std::string Server_handler::Err_NotOnChannel::get_str(void)
{
	return (str);
}
/*

else if (!split_line[0].compare("MODE") && split_line[1][0] == '#')
	{
		//msg.mode_msg(user, client_socket, split_line[1]);
		
	}

Server_handler::Err_NotOnChannel::Err_NotOnChannel(std::string str) : str(str)
{
	
}

std::string Server_handler::Err_NotOnChannel::get_str(void)
{
	return (str);
}
*/
