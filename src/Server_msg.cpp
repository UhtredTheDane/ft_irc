#include "../include/Server_msg.hpp"

/*
std::string Server_msg::welcome_msg(User user)
{
	std::string msg = ":irc.42.com ";
    msg += "001 ";
    msg += user.get_nickname();
    msg += " :Welcome to our irc server ";
    msg += user.get_identifier();
    msg += "\r\n";
	return(msg);
}

std::string Server_msg::yourhost_msg(User user)
{
	std::string msg = ":irc.42.com ";
    msg += "002 ";
    msg += user.get_nickname();
    msg += "Your host is ";
	msg += user.get_servername();
	msg += ", running version 0.02";
    msg += "\r\n";
	return(msg);
}

std::string Server_msg::created_msg(User user)
{
	std::string msg = ":irc.42.com ";
    msg += "003 ";
    msg += user.get_nickname();
    msg += " :This server was created 16-03-24 ";
    msg += "\r\n";
	return(msg);
}

std::string Server_msg::myinfo_msg(User user)
{
	std::string msg = ":irc.42.com ";
    msg += "004 ";
    msg += user.get_nickname();
	msg += " " + user.get_servername();
    msg += " :0.02 Bla blabla ";
    msg += "\r\n";
	return(msg);
}

std::string Server_msg::whois_msg(User user)
{
	std::string msg;
    msg += "311 ";
    msg += user.get_nickname();
	msg += " " + user.get_username();
	msg += " " + user.get_hostname();
    msg += " : " + user.get_realname();
    msg += "\r\n";
	return(msg);
}

std::string Server_msg::ping_msg(User user)
{
	std::string msg;
    msg += "PING ";
	msg += user.get_socket();
    msg += "\r\n";
	return(msg);
}

std::string Server_msg::pong_msg(User user)
{
	std::string msg;
    msg += "PONG localhost " ;
	msg += user.get_socket();
    msg += "\r\n";
	return(msg);
}*/
