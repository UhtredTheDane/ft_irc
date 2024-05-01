/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agengemb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 16:07:38 by agengemb          #+#    #+#             */
/*   Updated: 2024/04/30 13:19:50 by agengemb         ###   ########.fr       */
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
	request_types[11] = "TOPIC";

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
	requests_ptr[11] = &Server_handler::topic_request;

	//enum Chanmod { i = 1 , k = 2 , o = 3 , l = 4 , t = 5};

	options_types[0] = "i";
	options_types[1] = "k";
	options_types[2] = "l";
	options_types[3] = "o";
	options_types[4] = "t";

	options_ptr[0] = &Server_handler::i_request;
	options_ptr[1] = &Server_handler::k_request;
	options_ptr[2] = &Server_handler::l_request;
	options_ptr[3] = &Server_handler::o_request;
	options_ptr[4] = &Server_handler::t_request;
}	

Server_msg* Server_handler::get_msg(void)
{
	return (&msg);
}
void Server_handler::capls_request(User* user)
{
	if(split_line.size() < 2)
		throw(Err_NeedMoreParams("CAP"));
	else if(split_line[1] != "LS")
		throw(Err_NotRegistred(user->get_socket()));
	else if((user->get_isRegistered()) == 2)
		throw (Err_AlreadyRegistred());
	else if (user->get_isRegistered() == 0)
		user->set_isRegistered(1);
	else
		user->reset_userinfo();
}

void Server_handler::pass_request(User* user)
{
	if(split_line.size() < 2)
		throw(Err_NeedMoreParams("PASS"));
	else if(user->get_isRegistered() == 0)
		throw(Err_NotRegistred(user->get_socket()));
	else if((user->get_isRegistered()) == 2)
		throw (Err_AlreadyRegistred());
	else if((!serv->check_password(split_line[1])) && user->get_isRegistered() == 1)
		throw (Err_PasswordIncorrect());
	user->set_isPasswordValid(true);
}

void Server_handler::nick_request(User* user)
{
	if(split_line.size() < 2)
		throw(Err_NeedMoreParams("NICK"));
	if(user->get_isRegistered() == 0)
		throw(Err_NotRegistred(user->get_socket()));
	else if((user->get_isRegistered()) == 2)
		throw (Err_AlreadyRegistred());
	else if(user->get_isPasswordValid() && user->get_isRegistered() == 1)
	{
		if(serv->is_on_serv(split_line[1]))
			msg.nicknameinuse_msg(user, split_line[1]);
		else
		{
			user->set_nickname(split_line[1]);
			creation_done(user);
		}
	}
}

void Server_handler::topic_request(User* user)
{
	Channel *chan = NULL;
	std::string arg = "";
	std::string response = "";

	if(split_line.size() < 2)
		throw(Err_NeedMoreParams(this->raw_msg));
	if(chan->IsValidChannelName(split_line[1]))
	{
		try
		{
			chan = this->serv->get_channels().at(this->split_line[1]);
		}
		catch(std::exception& e)
		{
			// no such chan;
		}
	}
	if(!chan->IsInChannel(user))
		throw(Err_NotOnChannel(chan->get_name()));
	if(chan->IsOption(5))
	{	
		if(split_line.size() == 2)
		{
			if(chan->get_theme().size() > 0)
			{
				response = ":ircserv.42.fr ";
				response += RPL_TOPIC ;
				response += " "+ user->get_nickname() + " " + chan->get_name() + " " + chan->get_theme() +"\r\n"; 
			}
			else
			{
				response = ":ircserv.42.fr ";
				response += RPL_NOTOPIC ;
				response += /*" " + user->get_nickname()*/ + " " + chan->get_name() + " :No topic is set\r\n";
			}
			send(user->get_socket(), response.c_str(), response.length(), 0);
			return;
		}
		if(!chan->IsMod(user))
			throw(Err_chanoprivsneeded(chan->get_name()));
		if(split_line.size() > 2)
		{

			if(split_line.size() == 3 && split_line[2].size() == 1 && split_line[2][0] == ':') 
			{
				response = ":" + user->get_nickname() + "!" + user->get_nickname() + "@localhost" + " TOPIC " + chan->get_name() + " :\r\n";
				chan->set_topic("");
				chan->send_all(response);
				return;
			}
			else
			{
				std::vector<std::string>::iterator it = split_line.begin() + 2;
				while(it != split_line.end())
				{
					arg += *it + " ";
					it++;
				}
				chan->set_topic(arg);
				response = ":" + user->get_nickname() + "!" + user->get_nickname() + "@localhost";
				response += " TOPIC " + chan->get_name() + " " + arg + "\r\n";
				chan->send_all(response);
				return;
			}
		}
	}
	else
		throw(Err_NoChanModes(chan->get_name()));
}
void Server_handler::invite_request(User* user)
{
	User *target;
	Channel *chan;
	std::string reply;

	if(this->split_line.size() < 3)
		throw(Err_NeedMoreParams(this->raw_msg));//throw need more param
	else
	{
		try
		{
			chan = this->serv->get_channels().at(this->split_line[2]);//channel exist 
		}
		catch(const std::exception& e)
		{
			msg.nosuchchannel_msg(user,split_line[2]);//throw nosucchannel
			return;
		}
		if(!chan->IsInChannel(user))
			throw(Err_NotOnChannel(chan->get_name()));
		if(!chan->IsMod(user))
			throw(Err_chanoprivsneeded(chan->get_name()));
		
		target = chan->findUserByName(*chan->get_users(),this->split_line[1]) ;//user in the channel
		if(!target)
		{
			target = this->serv->findUserByName(this->split_line[1]);//user exist
			if(target)
			{	
				reply = ":" + user->get_nickname() + "!" + "" + " INVITE " + target->get_nickname() + " "+ chan->get_name() + "\r\n";
				if(!chan->IsOption(1))
					chan->invite_user(target);
				send(target->get_socket(),reply.c_str(),reply.length(),0);
			}
			else
				throw(Err_NoSuchNick(this->split_line[1]));
		}
		else
			throw(Err_useronchannel(this->split_line[1],this->split_line[2]));
	}
}

void Server_handler::user_request(User* user)
{
	if(user->get_isRegistered() == 0)
		throw(Err_NotRegistred(user->get_socket()));
	else if((user->get_isRegistered()) == 2)
		throw (Err_AlreadyRegistred());
	else if(split_line.size() < 5)
		throw(Err_NeedMoreParams("USER"));
	else if(user->get_isPasswordValid() && user->get_isRegistered() == 1 )
	{
		user->set_username(split_line[1]);
		user->set_hostname(split_line[2]);
		user->set_servername(split_line[3]);
		user->set_realname(split_line[3]);
		creation_done(user);
	}
	else
		throw(Err_NotRegistred(user->get_socket()));
}
void Server_handler::creation_done(User* user)
{
	if(!user->get_username().empty() && !user->get_hostname().empty() 
		&& !user->get_servername().empty() && !user->get_realname().empty() && !user->get_nickname().empty())
		{
			user->set_identifier();
		user->set_isRegistered(2);
		msg.welcome_msg(user);
		msg.yourhost_msg(user);
		msg.created_msg(user);
		msg.myinfo_msg(user);
		}
}

void Server_handler::pong_request(User* user)
{
	msg.pong_msg(user);
}

void Server_handler::join_request(User* user)
{
	if (split_line.size() < 2)
		throw(Err_NeedMoreParams(split_line[0]));
	std::stringstream list_name(split_line[1]);
	std::string channel_name;
	Channel* current_chan;
	while (getline(list_name, channel_name, ','))
	{
		if(channel_name[0] != '#')
		{
			throw(Err_NoSuchChannel(channel_name));
		}
		try
		{	
			current_chan = serv->get_channels().at(channel_name);
		}
		catch (std::out_of_range& oor)
		{
			msg.join_msg(user, serv->add_channel(channel_name, user));
			return ;
		}
		if (current_chan->IsOption(1) && !current_chan->erase_invite(user))
			throw(Err_InviteOnlyChan(channel_name));
		else if (current_chan->IsOption(2) && !current_chan->check_key(split_line[2]))
			throw(Err_BadChannelKey(channel_name));
		else if (current_chan->IsOption(4) && current_chan->is_full())
			throw(Err_ChannelIsFull(channel_name));
		if (!current_chan->IsInChannel(user))
		{
			current_chan->add_user(user);
			msg.join_msg(user, current_chan);
		}
	}
}

void Server_handler::i_request(User *user, std::vector<std::string> line,int *param,std::string *validparam,std::string *validoptions,Channel *target_chan)
{
	(void)user;
	(void)validparam;
	(void)param;
	if(line[2][0] == '-')
	{
		*validoptions += "i";
		target_chan->remove_mod(user,1);
	}
	else if(line[2][0] == '+')
	{
		*validoptions += "i";
		target_chan->set_mod(user,1);
	}
}

void Server_handler::o_request(User *user, std::vector<std::string> line,int *param,std::string *validparam,std::string *validoptions,Channel *target_chan)
{
	if(*param == -1 || *param >= (int)line.size())
		throw(Err_NeedMoreParams(this->raw_msg));
	if(line[2][0] == '-')
	{
		if(line[*param].c_str() && target_chan->take_privilege(user ,line[*param]) == 0)
		{
			*validoptions += "o";
			*validparam += line[*param] + " ";
			*param = *param + 1 ;
		}
	}
	else if(line[2][0] == '+')
	{
		if(line[*param].c_str())
		{
			try{
				if(target_chan->give_privilege(user,line[*param]) == 0)
				{
					*validoptions += "o";
					*validparam += line[*param] + " ";
					*param = *param + 1 ;
				}
				else
					throw(Err_UserNotInChannel(line[*param],target_chan->get_name()));
			}
			catch(Channel::AlreadyMod &e)
			{
				return;
			}
			catch(Err_UserNotInChannel &e)
			{
				throw(Err_UserNotInChannel(line[*param],target_chan->get_name()));
			}
		}
	}				
}

void Server_handler::l_request(User *user, std::vector<std::string> line,int *param,std::string *validparam,std::string *validoptions,Channel *target_chan)
{
	if(*param == -1 || *param >= (int)line.size())
		throw(Err_NeedMoreParams(this->raw_msg));
	if(line[2][0] == '-')
	{
		if(!target_chan->remove_mod(user,4))
		{
			*validoptions += "l";
			target_chan->set_limit_user(-1);
		}
	}
	else if(line[2][0] == '+')
	{
		if(line[*param].c_str())
		{
			try
			{
				if(!target_chan->set_mod(user,4))
				{
					int nb = std::atoi(line[*param].c_str());
					target_chan->set_limit_user (nb) ;
					*validparam += line[*param] + " ";
					*param = *param + 1 ;
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

void Server_handler::k_request(User *user, std::vector<std::string> line,int *param,std::string *validparam,std::string *validoptions,Channel *target_chan)
{
	if(*param == -1 || *param >= (int)line.size())
		throw(Err_NeedMoreParams(this->raw_msg));
	if(line[2][0] == '-')
	{
		if(!target_chan->remove_mod(user,2))
		{
			*validoptions += "k";
			target_chan->set_password(NULL);
		}
	}
	else if(line[2][0] == '+')
	{
		if(line[*param].c_str())
		{
			if(!target_chan->set_mod(user,2))
			{
				*validoptions += "k";
				target_chan->set_password(line[*param]);
				*validparam += line[*param] + " ";
				*param = *param + 1 ;
			}
		}
	}
}

void Server_handler::t_request(User *user, std::vector<std::string> line,int *param,std::string *validparam,std::string *validoptions,Channel *target_chan)
{
	(void)param;
	(void)validparam;
	if(line[2][0] == '-')
	{
		if(!target_chan->remove_mod(user,5))
		{
			*validoptions += "t";
		}
	}
	else if(line[2][0] == '+')
	{
		if(!target_chan->set_mod(user,5))
		{
			*validoptions += "t";
		}
	}

}

void Server_handler::update_mod(User *user, std::vector<std::string> line,Channel *target_chan)
{
	std::string options;
	int param = -1;
	std::string response = "";
	std::string validoptions = "";
	std::string validparam = "";
	std::string::iterator current;

	if(line.size() < 2)
		throw(Err_NeedMoreParams(this->raw_msg)); //ERR_NEEDMOREPARAMS 
	if(!target_chan->IsValidChannelName(line[1]))
		return;
	if(line.size() == 2)
		return;
	if(!target_chan->IsMod(user))
		throw(Err_chanoprivsneeded(line[1])); //ERR_CHANOPRIVSNEEDED
	options = line[2];
	if (line.size() >= 4)
		param = 3;
	if(options.size() >= 2)
	{
		if(options[0] == '-' || options[0] == '+')
		{
			current = options.begin();
			validoptions += options[0];
			current++;
			while (current != options.end())
			{
				int i = 0;
				while(i < 5)
				{
					if(*(options_types[i].c_str()) == *current)
					{	
						(this->*options_ptr[i])( user, line, &param, &validparam, &validoptions,target_chan);
						break;
					}
					i++;
				}
				if(i == 5)
					throw(Err_UnknowedMode(*current,target_chan->get_name()));
				current ++;
			}
		}
		if(validoptions.size() > 1)
		{
			response = ":" + user->get_nickname() + "!" + user->get_nickname() + "@localhost";
			response += " MODE " + line[1] + " " + validoptions + " " + validparam + "\r\n";
			target_chan->send_all(response);
		}
	}
}




void Server_handler::mode_request(User* user)
{
	if(split_line.size() < 2)
		throw(Err_NeedMoreParams(this->raw_msg));
	try
	{
		Channel* target_chan = serv->get_channels().at(split_line[1]);
		update_mod(user,split_line,target_chan);
	}
	catch (std::out_of_range& oor)
	{
		throw (Err_NoSuchChannel(split_line[1]));
	}
}

void Server_handler::kick_request(User* user)
{
	int i = 0;
	Channel *curent_chan;
	if(split_line.size() < 3)
		throw(Err_NeedMoreParams(split_line[0]));
	try
	{
		curent_chan = serv->get_channels().at(split_line[1]);
	}
	catch(std::out_of_range& oor)
	{
		throw(Err_NoSuchChannel(split_line[1]));
	}
	for (std::vector<User*>::iterator it = curent_chan->get_admins()->begin(); it != curent_chan->get_admins()->end();)
	{
		if(user == *it)
			i = 1;
		it++;
	}
	if(i == 0)
		throw(Err_chanoprivsneeded(curent_chan->get_name()));
	i = 0;
	for (std::vector<User*>::iterator it = curent_chan->get_users()->begin(); it != curent_chan->get_users()->end();)
	{		
		if(user == *it)
			i = 1;
		it++;
	}
	if(i == 0)
		throw(Err_NotOnChannel(this->split_line[1]));/*ERR_NOTONCHANNEL*/;
	if(msg.kick_msg(user, curent_chan,split_line) == -1)
		throw(Err_UserNotInChannel(split_line[2], split_line[1]))/*ERR_USERNOTINCHANNEL */;
}

void Server_handler::privmsg_request(User* user)
{
	Channel *curent_chan;
	int i = 0;
	if(split_line.size() < 3)
		throw(Err_NeedMoreParams(split_line[0]));
	if(split_line[1][0] == '#')
	{
	try
	{
		curent_chan = serv->get_channels().at(split_line[1]);
	}
	catch (std::out_of_range& oor)
	{
		throw(Err_CannotSendToChan(split_line[1]));
	}
	for (std::vector<User*>::iterator it = curent_chan->get_users()->begin(); it != curent_chan->get_users()->end();)
	{		
		if(user == *it)
		{
			i = 1;
		}
		it++;
	}
	if(i == 0)
	{
		throw(Err_NotOnChannel(this->split_line[1]));
	}
		std::map<int, User*> users_map = serv->get_users();
		Message c_msg(split_line[2], user);
		//curent_chan->add_message(&c_msg);
		msg.chan_msg(user, curent_chan, split_line);
	}
	else
	{
		std::map<int, User*> users_map = serv->get_users();
		if(msg.priv_msg(user, split_line, users_map) == -1)
			throw(Err_NoSuchNick(split_line[1]));
	}

}

void Server_handler::part_request(User* user)
{
		if(split_line.size() < 2)
			throw(Err_NeedMoreParams(this->raw_msg));
		try
		{
			Channel *current_chan = serv->get_channels().at(split_line[1]);
			msg.leave_msg(user, current_chan);
			current_chan->delete_user(user);
		}
		catch (std::out_of_range& oor)
		{
			throw (Err_NoSuchChannel(split_line[1]));
		}
}

void Server_handler::processing_request(User* user, std::string& request)
{
	std::stringstream coco(request);
	std::string word;
	int i;
	while (getline(coco, word, ' '))
		split_line.push_back(word);
	for (i = 0; i < 12; ++i)
	{
		if(i > 3 && user->get_isRegistered() != 2)
		{
			i = 0;
			split_line.clear();
			throw(Err_NotRegistred(user->get_socket()));
		}
		else if (!split_line[0].compare(request_types[i]))
		{
			try
			{
				(this->*requests_ptr[i])(user);
			}
			catch (AException& e)
			{
				e.handle(user, &msg);
			}
			break;
		}
	}
	split_line.clear();
	if(i == 12)
		throw(Err_UnknownCommand(request));
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
				catch (Err_NotRegistred& e)
				{
					user->set_isRegistered(0);
					get_msg()->notregistred_msg(client_socket);
				}
				catch(Err_UnknownCommand& e)
				{
					e.handle(user, &msg);
				}
				user->buffer.erase(0, delim_pos + delimiter.length());
			}
		}
		else
			user->buffer += request;
	}
	catch(std::out_of_range& oor)
	{
	}

}

User *Server_handler::findUserByName(std::vector<User *> v,std::string name)
{
	for(std::vector<User *>::iterator it  = v.begin(); it != v.end();it ++)
		if((*it)->get_nickname() == name)
			return *it;
	return(NULL);
}
