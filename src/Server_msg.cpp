#include "../include/Server_msg.hpp"
 
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
	std::cout << "msg :" << msg << std::endl;
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

void Server_msg::priv_msg(User* user, std::map<int, User*> users_map)
{
	std::string p_msg;
	Channel *curent_chan = serv->get_channels().at(split_line[1]);
	for (std::vector<User*>::iterator it = curent_chan->get_users()->begin(); it != curent_chan->get_users()->end();)
	{
		if(*it != user)
		{
			p_msg = ":" + user->get_nickname() + " PRIVMSG " + it->second->get_nickname()+ " " + split_line[2] + "\r\n";
			std::cout << p_msg << std::endl;
			send(it->first, p_msg.c_str(), p_msg.length(), 0);
			break;
		}
		it++;
	}
	std::string p_msg;

}

void Server_msg::chan_msg(User* user, Channel *curent_chan)
{
	std::map<int, User*> users_map = serv->get_users();
	std::string c_msg;
	for (std::map<int, User*>::iterator it = users_map.begin(); it != users_map.end(); ++it)
	{
		if (it->second->get_nickname() == split_line[1])
		{
			c_msg = ":" + user->get_identifier() + " PRIVMSG " + curent_chan->get_name()+ " " + get_msg() + "\r\n";
			std::cout << c_msg << std::endl;
			send((*it)->get_socket(), c_msg.c_str(), c_msg.length(), 0);
			break;
		}
	}	

}