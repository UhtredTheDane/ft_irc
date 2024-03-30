#include "../include/Server_msg.hpp"

Server_msg::Server_msg()
{
	request_types = {"PING", "JOIN", "MODE", "KICK", "PRIVMSG", "PART"};
	requests_ptr[0] = &Server_msg::pong_request;
	requests_ptr[1] = &Server_msg::join_request;
	requests_ptr[2] = &Server_msg::mode_request;
	requests_ptr[3] = &Server_msg::kick_request;
	requests_ptr[4] = &Server_msg::privmsg_request;
	requests_ptr[5] = &Server_msg::part_request;
}

void Server_msg::processing_request(int client_socket, User* user, std::string& request)
{
	std::stringstream coco(request);
	std::vector<std::string> split_line;
	std::string word;

	while (getline(coco, word, ' '))
		split_line.push_back(word);
	if (user->get_isRegistered() == 0 && !request.compare("CAP LS"))
	{
		user->set_isRegistered(1);
	}
	else if(user->get_isRegistered() == 1)
	{
		if (!split_line[0].compare("PASS"))
		{
			std::cout << "PASS valide\n";
		}
		else if (!split_line[0].compare("NICK"))
		{
			while (is_on_serv(split_line[1]))
				split_line[1] += "_";
			user->set_nickname(split_line[1]);
		}
		else if (!split_line[0].compare("USER") && !user->get_nickname().empty())
		{
				user->set_username(split_line[1]);
				user->set_hostname(split_line[2]);
				user->set_servername(split_line[3]);
				user->set_realname(split_line[3]);
				user->set_socket(client_socket);
				user->set_identifier();
				// user.show_userinfo(user);
				reply(user);
				user->set_isRegistered(2);
		}
	}

	for (int i = 0; i < 6; ++i)
	{
		if (!split_line[0].compare(msg.get_request_type(i)))
		{
			*requests_ptr[i](user);
		}
	}
	
}

std::string Server_msg::get_request_type(size_t i)
{
	return (request_types[i]);
}

std::string Server_msg::get_request_ptr(size_t i)
{
	return (request_types[i]);
}

void Server_msg::pong_request(User* user)
{
	pong_msg(user);
}

void Server_msg::join_request(User* user)
{
		try
		{	
			std::cout << user->get_username() << std::endl;
			Channel* current_chan = channels.at(split_line[1]);
			current_chan->add_user(user);
			join_msg(user, current_chan);

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
			Channel* new_chan = new Channel(split_line[1], user);
			channels.insert(std::pair<std::string, Channel*>(split_line[1], new_chan));
			join_msg(user, new_chan);
		}
}

void Server_msg::mode_request(User* user)
{
		try
		{
			Channel* target_chan = channels.at(split_line[1]);
			mode_msg(user, target_chan);
		}
		catch (std::out_of_range& oor)
		{
		}
}

void Server_msg::kick_request(User* user)
{
		Channel *curent_chan = channels.at(split_line[1]);
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

void Server_msg::privmsg_request(User* user)
{
	try
		{
			if(split_line[1][0] == '#')
			{
				Channel *curent_chan = channels.at(split_line[1]); 
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

void Server_msg::part_request(User* user)
{
		try
		{
			Channel *current_chan = channels.at(split_line[1]); 
			leave_msg(user, current_chan);
			current_chan->delete_user(user);
		}
		catch (std::out_of_range& oor)
		{
		}
}

void Server_msg::welcome_msg(User* user)
{
	std::string msg = ":irc.42.com ";
	msg += "001 ";
	msg += user->get_nickname();
	msg += " :Welcome to our irc server ";
	msg += user->get_identifier();
	msg += "\r\n";
	int msg_len = msg.length();
	send(user->get_socket(), msg.c_str(), msg_len, 0);
}

void Server_msg::yourhost_msg(User* user)
{
	std::string msg = ":irc.42.com ";
	msg += "002 ";
	msg += user->get_nickname();
	msg += "Your host is ";
	msg += user->get_servername();
	msg += ", running version 0.02";
	msg += "\r\n";
	int msg_len = msg.length();
	send(user->get_socket(), msg.c_str(), msg_len, 0);
}

void Server_msg::created_msg(User* user)
{
	std::string msg = ":irc.42.com ";
	msg += "003 ";
	msg += user->get_nickname();
	msg += " :This server was created 16-03-24 ";
	msg += "\r\n";
	int msg_len = msg.length();
	send(user->get_socket(), msg.c_str(), msg_len, 0);
}

void Server_msg::myinfo_msg(User* user)
{
	std::string msg = ":irc.42.com ";
	msg += "004 ";
	msg += user->get_nickname();
	msg += " " + user->get_servername();
	msg += " :0.02 Bla blabla ";
	msg += "\r\n";
	int msg_len = msg.length();
	send(user->get_socket(), msg.c_str(), msg_len, 0);
}

void Server_msg::whois_msg(User* user)
{
	std::string msg;
	msg += "311 ";
	msg += user->get_nickname();
	msg += " " + user->get_username();
	msg += " " + user->get_hostname();
	msg += " : " + user->get_realname();
	msg += "\r\n";
	int msg_len = msg.length();
	send(user->get_socket(), msg.c_str(), msg_len, 0);
}

void Server_msg::ping_msg(User* user)
{
	std::string msg;
	int client_socket = user->get_socket();
	msg += "PING ";
	msg += client_socket;
	msg += "\r\n";
	int msg_len = msg.length();
	send(client_socket, msg.c_str(), msg_len, 0);
}
 
void Server_msg::pong_msg(User* user)
{
	int client_socket = user->get_socket();
	std::string msg = ":" + user->get_nickname() + "!" + user->get_nickname() + "@localhost ";
	msg += "PONG localhost " ;
	msg += client_socket;
	msg += "\r\n";
	int msg_len = msg.length();
	send(client_socket, msg.c_str(), msg_len, 0);
}

void Server_msg::join_msg(User *user, Channel* channel)
{
	std::string msg;
	msg += ":" + user->get_nickname() + "!" + user->get_nickname() + "@localhost JOIN :" + channel->get_name();
	msg += "\r\n";

	msg += ":" + user->get_servername();
	msg += " 353 " + user->get_nickname() + " = " + channel->get_name() + " :@";
	for (std::vector<User *>::iterator it = channel->get_users()->begin(); it != channel->get_users()->end(); ++it)
	{
		msg += (*it)->get_nickname();
		if (it + 1 != channel->get_users()->end())
			msg += " ";
	}
	msg += "\r\n";

	msg += ":" + user->get_servername();
	msg += " 366 " + user->get_nickname() + " " + channel->get_name() + " :End of NAMES list";
	msg += "\r\n";
	int msg_len = msg.length();
	send(user->get_socket(), msg.c_str(), msg_len, 0);
}

void Server_msg::mode_msg(User* user, Channel* channel)
{
	std::string msg = ":" + user->get_servername();
	msg += " 324 " + user->get_username() + " " + channel->get_name() + " +" ;
	msg += "\r\n";
	int msg_len = msg.length();
	send(user->get_socket(), msg.c_str(), msg_len, 0);
}

void Server_msg::leave_msg(User* user, Channel* channel)
{
	std::string msg = ":" + user->get_nickname() + "!" + user->get_nickname() + "@localhost PART ";
	msg += channel->get_name();
	std::string str = channel->get_name();
	str[0] = ':';
	msg += " " + str + "\r\n";
	int msg_len = msg.length();
	
	for (std::vector<User*>::iterator it = channel->get_users()->begin(); it != channel->get_users()->end(); ++it)
		send((*it)->get_socket(), msg.c_str(), msg_len, 0);
}
