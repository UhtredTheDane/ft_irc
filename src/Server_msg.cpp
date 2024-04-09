#include "../include/Server_msg.hpp"
 
void Server_msg::welcome_msg(User* user)
{
	std::string msg;

	msg = ":irc.42.com " + "001 " + user->get_nickname() + " :Welcome to our irc server " + user->get_identifier() + "\r\n";
	send(user->get_socket(), msg.c_str(), msg.length(), 0);
}

void Server_msg::yourhost_msg(User* user)
{
	std::string msg;

	msg = ":irc.42.com " + "002 " + user->get_nickname() + "Your host is " + user->get_servername() + ", running version 0.02\r\n";
	send(user->get_socket(), msg.c_str(), msg.length(), 0);
}

void Server_msg::created_msg(User* user)
{
	std::string msg;

	msg = ":irc.42.com " + "003 " + user->get_nickname() + " :This server was created 16-03-24 \r\n";
	send(user->get_socket(), msg.c_str(), msg.length(), 0);
}

void Server_msg::myinfo_msg(User* user)
{
	std::string msg;
	
	msg = ":irc.42.com " + "004 " + user->get_nickname() + " " + user->get_servername() + " :0.02 Bla blabla \r\n";
	send(user->get_socket(), msg.c_str(), msg.length(), 0);
}

void Server_msg::whois_msg(User* user)
{
	std::string msg;

	msg += "311 " + user->get_nickname() + " " + user->get_username() + " " + user->get_hostname();
	msg += " : " + user->get_realname() += "\r\n";
	send(user->get_socket(), msg.c_str(), msg.length(), 0);
}

void Server_msg::ping_msg(User* user)
{
	std::string msg;
	int client_socket;

	client_socket = user->get_socket();
	msg = "PING " + client_socket + "\r\n";
	send(client_socket, msg.c_str(), msg.length(), 0);
}
 
void Server_msg::pong_msg(User* user)
{
	int client_socket;
	std::string msg;

	client_socket = user->get_socket();
	msg = ":" + user->get_nickname() + "!" + user->get_nickname() + "@localhost " + "PONG localhost " ;
	msg += client_socket + "\r\n";
	send(client_socket, msg.c_str(), msg.length(), 0);
}


void Server_msg::join_msg(User *user, Channel* channel)
{
	std::string msg;
	int msg_len;

	msg = ":" + user->get_nickname() + "!" + user->get_nickname() + "@localhost JOIN :" + channel->get_name() + "\r\n";
	msg_len = msg.length();
	for (std::vector<User*>::iterator it = current_chan->get_users()->begin(); it != current_chan->get_users()->end(); ++it)
	{
			if (user != *it)
				send((*it)->get_socket(), msg.c_str(), msg_len, 0);
	}
	msg += ":" + user->get_servername() + " 353 " + user->get_nickname() + " = " + channel->get_name() + " :@";
	for (std::vector<User *>::iterator it = channel->get_users()->begin(); it != channel->get_users()->end(); ++it)
	{
		msg += (*it)->get_nickname();
		if (it + 1 != channel->get_users()->end())
			msg += " ";
	}
	msg += "\r\n";
	msg += ":" + user->get_servername() + " 366 " + user->get_nickname() + " " + channel->get_name() + " :End of NAMES list\r\n";
	msg_len = msg.length();
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
	std::string msg;
	
	msg = ":" + user->get_nickname() + "!" + user->get_nickname() + "@localhost PART " + channel->get_name();
	std::string str = channel->get_name();
	str[0] = ':';
	msg += " " + str + "\r\n";
	int msg_len = msg.length();
	
	for (std::vector<User*>::iterator it = channel->get_users()->begin(); it != channel->get_users()->end(); ++it)
		send((*it)->get_socket(), msg.c_str(), msg_len, 0);
}
