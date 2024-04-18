#include "../include/Server_msg.hpp"
#include "../include/reply_macros.hpp"
#include "../include/reply_macros_error.hpp"

void Server_msg::welcome_msg(User* user)
{
	std::string msg = ":irc.42.com 001 " + user->get_nickname();
	msg += " :Welcome to our irc server " + user->get_identifier() + "\r\n";
	send(user->get_socket(), msg.c_str(), msg.length(), 0);
}

void Server_msg::yourhost_msg(User* user)
{
	std::string msg = ":irc.42.com 002 " + user->get_nickname() + "Your host is "; 
	msg += user->get_servername() + ", running version 0.02\r\n";
	send(user->get_socket(), msg.c_str(), msg.length(), 0);
}

void Server_msg::created_msg(User* user)
{
	std::string msg = ":irc.42.com 003 " + user->get_nickname();
       	msg += " :This server was created 16-03-24 \r\n";
	send(user->get_socket(), msg.c_str(), msg.length(), 0);
}

void Server_msg::myinfo_msg(User* user)
{
	std::string msg = ":irc.42.com 004 " + user->get_nickname();
	msg += " " + user->get_servername() + " :0.02 Bla blabla \r\n";
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
	int client_socket;
	client_socket = user->get_socket();
	std::string msg = "PING ";
	msg += client_socket;
	msg += "\r\n";
	send(client_socket, msg.c_str(), msg.length(), 0);
}
 
void Server_msg::pong_msg(User* user)
{
	int client_socket;
	client_socket = user->get_socket();
	std::string msg = ":" + user->get_nickname() + "!" + user->get_nickname();
	msg += "@localhost PONG localhost ";
	msg += client_socket;
	msg += "\r\n";
	send(client_socket, msg.c_str(), msg.length(), 0);
}


void Server_msg::join_msg(User *user, Channel* channel)
{
	std::string msg;
	int msg_len;

	msg += ":" + user->get_nickname() + "!" + user->get_nickname() + "@localhost JOIN :" + channel->get_name() + "\r\n";
	msg_len = msg.length();
	for (std::vector<User*>::iterator it = channel->get_users()->begin(); it != channel->get_users()->end(); ++it)
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
	
	msg += ":" + user->get_nickname() + "!" + user->get_nickname() + "@localhost PART " + channel->get_name();
	std::string str = channel->get_name();
	str[0] = ':';
	msg += " " + str + "\r\n";
	int msg_len = msg.length();
	
	for (std::vector<User*>::iterator it = channel->get_users()->begin(); it != channel->get_users()->end(); ++it)
		send((*it)->get_socket(), msg.c_str(), msg_len, 0);
}

void Server_msg::passwordincorrect_msg(User* user)
{
	std::string msg = ":irc.42.com 464 * :Password incorrect\r\n";
	send(user->get_socket(), msg.c_str(), msg.length(), 0);
}

void Server_msg::alreadyregistred_msg(User* user)
{
	std::string msg = ":irc.42.com 451 * :You may not reregister\r\n";
	send(user->get_socket(), msg.c_str(), msg.length(), 0);
}

void Server_msg::nosuchchannel_msg(User* user, std::string& channel_name)
{
	std::string msg = ":irc.42.com 403" + user->get_nickname() + " ";
	msg += channel_name + " :No such channel\r\n";
	send(user->get_socket(), msg.c_str(), msg.length(), 0);
}

void Server_msg::needmoreparams_msg(User* user, std::string& command)
{
	std::string msg = ":irc.42.com 461" + user->get_nickname() + " " + command + " :Not enough parameters\r\n";
	send(user->get_socket(), msg.c_str(), msg.length(), 0);
}
//ERR_NOTONCHANNEL  
void Server_msg::notonchannel_msg(User* user, std::string& channel_name)
{
	std::string msg = ":irc.42.com 442" + user->get_nickname() + " ";
	msg += channel_name + " :You're not on that channel\r\n";
	send(user->get_socket(), msg.c_str(), msg.length(), 0);
}

void Server_msg::err_keyset_msg(User* user, std::string& channel_name)
{
	std::string msg = ":irc.42.com " ;
	msg += ERR_KEYSET " ";
	msg += user->get_nickname() + " ";
	msg += channel_name + " :Channel key already set\r\n";
	send(user->get_socket(), msg.c_str(), msg.length(), 0);
}

void Server_msg::err_chanoprivneeded_msg(User* user, std::string& channel_name)
{
	std::string msg = ":irc.42.com " ;
	msg += ERR_CHANOPRIVSNEEDED " ";
	msg += user->get_nickname() + " ";
	msg += channel_name + " :You're not channel operator\r\n";
	send(user->get_socket(), msg.c_str(), msg.length(), 0);
}

 void Server_msg::err_usernotinchannel_msg(User* user, std::string& channel_name, std::string& nick)
{
	std::string msg = ":irc.42.com " ;
	msg += ERR_USERNOTINCHANNEL " ";
	msg += user->get_nickname() + " ";
	msg += nick + " " + channel_name + " :They aren't on that channel\r\n";
	send(user->get_socket(), msg.c_str(), msg.length(), 0);
}

void Server_msg::err_unknowmode_msg(User* user,std::string& channel_name ,std::string& option)
{
	std::string msg = ":irc.42.com " ;
	msg += ERR_UNKNOWNMODE " ";
	msg += user->get_nickname() + " ";
	msg += option + " :is unknown mode char to me for " + channel_name + "\r\n";
	send(user->get_socket(), msg.c_str(), msg.length(), 0);
}
void Server_msg::err_nosuchnick_msg(User* user,std::string& nick)
{
	std::string msg = ":irc.42.com " ;
	msg += ERR_NOSUCHNICK " ";
	msg += user->get_nickname() + " ";
	msg += nick + " :No such nick/channel\r\n";
	send(user->get_socket(), msg.c_str(), msg.length(), 0);
}
void Server_msg::err_useronchannel_msg(User* user,std::string& channel_name, std::string& nick)
{
	std::string msg = ":irc.42.com " ;
	msg += ERR_USERONCHANNEL " ";
	msg += user->get_nickname() + " ";
	msg += nick +" " + channel_name+ " :is already on channel\r\n";
	send(user->get_socket(), msg.c_str(), msg.length(), 0);
}