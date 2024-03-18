#include "../include/Server_msg.hpp"

void Server_msg::welcome_msg(User* user, int client_socket)
{
	std::string msg = ":irc.42.com ";
	msg += "001 ";
	msg += user->get_nickname();
	msg += " :Welcome to our irc server ";
	msg += user->get_identifier();
	msg += "\r\n";
	int msg_len = msg.length();
	send(client_socket, msg.c_str(), msg_len, 0);
}

void Server_msg::yourhost_msg(User* user, int client_socket)
{
	std::string msg = ":irc.42.com ";
	msg += "002 ";
	msg += user->get_nickname();
	msg += "Your host is ";
	msg += user->get_servername();
	msg += ", running version 0.02";
	msg += "\r\n";
	int msg_len = msg.length();
	send(client_socket, msg.c_str(), msg_len, 0);
}

void Server_msg::created_msg(User* user, int client_socket)
{
	std::string msg = ":irc.42.com ";
	msg += "003 ";
	msg += user->get_nickname();
	msg += " :This server was created 16-03-24 ";
	msg += "\r\n";
	int msg_len = msg.length();
	send(client_socket, msg.c_str(), msg_len, 0);
}

void Server_msg::myinfo_msg(User* user, int client_socket)
{
	std::string msg = ":irc.42.com ";
	msg += "004 ";
	msg += user->get_nickname();
	msg += " " + user->get_servername();
	msg += " :0.02 Bla blabla ";
	msg += "\r\n";
	int msg_len = msg.length();
	send(client_socket, msg.c_str(), msg_len, 0);
}

void Server_msg::whois_msg(User* user, int client_socket)
{
	std::string msg;
	msg += "311 ";
	msg += user->get_nickname();
	msg += " " + user->get_username();
	msg += " " + user->get_hostname();
	msg += " : " + user->get_realname();
	msg += "\r\n";
	int msg_len = msg.length();
	send(client_socket, msg.c_str(), msg_len, 0);
}

void Server_msg::ping_msg(int client_socket)
{
	std::string msg;
	msg += "PING ";
	msg += client_socket;
	msg += "\r\n";
	int msg_len = msg.length();
	send(client_socket, msg.c_str(), msg_len, 0);
}

void Server_msg::pong_msg(int client_socket)
{
	std::string msg;
	msg += "PONG localhost " ;
	msg += client_socket;
	msg += "\r\n";
	int msg_len = msg.length();
	send(client_socket, msg.c_str(), msg_len, 0);
}
