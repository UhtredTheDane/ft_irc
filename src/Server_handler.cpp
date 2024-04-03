/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agengemb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 16:07:38 by agengemb          #+#    #+#             */
/*   Updated: 2024/04/03 17:59:24 by agengemb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/Server_handler.hpp"

Server_handler::Server_handler(void)
{
    	msg = Server_msg();
	request_types[0] = "CAP LS";
	request_types[1] = "PASS";
	request_types[2] = "NICK";
	request_types[3] = "USER";
    	request_types[4] = "PING";
	request_types[5] = "JOIN";
	request_types[6] = "MODE";
	request_types[7] = "KICK"; 
	request_types[8] = "PRIVMSG";
	request_types[9] = "PART";

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
}

std::map<std::string, Channel*> Server_handler::get_channels(void)
{
	return (channels);
}

std::map<int, User*> Server_handler::get_users(void)
{
	return (users_map);
}

User* Server_handler::add_user(int fd_client)
{
	User* new_user = new User(fd_client);
	users_map.insert(std::pair<int, User*>(fd_client, new_user));
	return (new_user);
}

void Server_handler::delete_user(int fd_client)
{
	users_map.erase(fd_client);
}

Channel* Server_handler::add_channel(std::string name, User* user)
{
	Channel* new_chan = new Channel(name, user);
	channels.insert(std::pair<std::string, Channel*>(name, new_chan));
	return (new_chan);
}
void Server_handler::capls_request(User* user)
{
	if (user->get_isRegistered() == 0)
	{
		std::cout << "Salut\n";
		user->set_isRegistered(1);
	}
}

void Server_handler::pass_request(User* user)
{
	std::cout << user->get_isRegistered() << std::endl;
	if(user->get_isRegistered() == 1)
	{
		std::cout << "toto\n";
		std::cout << "PASS valide\n";
	}
}

void Server_handler::nick_request(User* user)
{
	if(user->get_isRegistered() == 1)
	{
		std::cout << "coco\n";
		while (is_on_serv(split_line[1]))
				split_line[1] += "_";
			user->set_nickname(split_line[1]);
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
		std::cout << "popo\n";
		msg.welcome_msg(user);
		msg.yourhost_msg(user);
		msg.created_msg(user);
		msg.myinfo_msg(user);
		user->set_isRegistered(2);
	}
}

void Server_handler::pong_request(User* user)
{
	msg.pong_msg(user);
}

void Server_handler::join_request(User* user)
{
		try
		{	
			std::cout << user->get_username() << std::endl;
			Channel* current_chan = get_channels().at(split_line[1]);
			current_chan->add_user(user);
			msg.join_msg(user, current_chan);

			std::string join_msg;
			join_msg += ":" + user->get_nickname() + "!" + user->get_nickname() + "@localhost JOIN :" + current_chan->get_name();
			join_msg += "\r\n";
			for (std::vector<User*>::iterator it = current_chan->get_users()->begin(); it != current_chan->get_users()->end(); ++it)
			{
				if (user != *it)
					send((*it)->get_socket(), join_msg.c_str(), join_msg.length(), 0);
			}

		}
		catch (std::out_of_range& oor)
		{
			msg.join_msg(user, add_channel(split_line[1], user));
		}
}

void Server_handler::mode_request(User* user)
{
		try
		{
			Channel* target_chan = get_channels().at(split_line[1]);
			msg.mode_msg(user, target_chan);
		}
		catch (std::out_of_range& oor)
		{
		}
}

void Server_handler::kick_request(User* user)
{
		Channel *curent_chan = get_channels().at(split_line[1]);
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
			if(split_line[1][0] == '#')
			{
				Channel *curent_chan = get_channels().at(split_line[1]); 
				Message msg(split_line[2], user);
				curent_chan->add_message(&msg);
			std::string c_msg;
			for (std::vector<User*>::iterator it = curent_chan->get_users()->begin(); it != curent_chan->get_users()->end();)
			{
				if(*it != user)
				{
				c_msg = ":" + user->get_identifier() + " PRIVMSG " + curent_chan->get_name()+ " " + msg.get_msg() + "\r\n";
				std::cout << c_msg << std::endl;
				send((*it)->get_socket(), c_msg.c_str(), c_msg.length(), 0);
				}
			it++;
			}
			}
			else
			{
                std::map<int, User*> users_map = get_users();
			for (std::map<int, User*>::iterator it = users_map.begin(); it != users_map.end(); ++it)
			{
				if (it->second->get_nickname() == split_line[1])
				{
					std::string p_msg;
					p_msg = ":" + user->get_nickname() + " PRIVMSG " + it->second->get_nickname()+ " " + split_line[2] + "\r\n";
					std::cout << p_msg << std::endl;
					send(it->first, p_msg.c_str(), p_msg.length(), 0);
					break;
				}
			}
			}
		}
		catch (std::out_of_range& oor)
		{
		}
}

void Server_handler::part_request(User* user)
{
		try
		{
			Channel *current_chan = get_channels().at(split_line[1]); 
			msg.leave_msg(user, current_chan);
			current_chan->delete_user(user);
		}
		catch (std::out_of_range& oor)
		{
		}
}

void Server_handler::processing_request(User* user, std::string& request)
{
	std::stringstream coco(request);
	std::string word;

	while (getline(coco, word, ' '))
		split_line.push_back(word);
	for (int i = 0; i < 10; ++i)
	{
		if (!request.compare(request_types[i]))
		{
			(this->*requests_ptr[i])(user);
			break;
		}
	}
	split_line.clear();
}

bool Server_handler::is_on_serv(std::string& nickname)
{
	try
	{
    std::map<int, User*> users_map = get_users();
	for (std::map<int, User*>::iterator it = users_map.begin(); it != users_map.end(); ++it)
	{
		if (it->second->get_nickname() == nickname)
			return (true);
	}
	}
	catch (std::out_of_range& oor)
	{
	}
	return (false);
}


void Server_handler::request_handler(int client_socket, std::string& request)
{
	User* user;
	std::string delimiter = "\r\n";
	std::string token;
	size_t delim_pos;

    	std::map<int, User*> users_map = get_users();
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
