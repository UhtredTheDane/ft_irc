#include "../include/IrcExceptions.hpp"

void Err_PasswordIncorrect::handle(User* user, Server_msg* msg)
{
	msg->passwordincorrect_msg(user);
}

void Err_AlreadyRegistred::handle(User* user, Server_msg* msg)
{
	msg->alreadyregistred_msg(user);
}

Err_NotRegistred::Err_NotRegistred(int socket) 
{
	this->socket = socket;
}

void Err_NotRegistred::handle(User* user, Server_msg* msg)
{
	(void) user;
	msg->notregistred_msg(get_socket());
}

Err_NeedMoreParams::Err_NeedMoreParams(std::string channel)
{
	this->channel = channel;
}

void Err_NeedMoreParams::handle(User* user, Server_msg* msg)
{
	msg->needmoreparams_msg(user, get_channel());
}

Err_UnknownCommand::Err_UnknownCommand(std::string channel)
{
	this->channel = channel;
}

void Err_UnknownCommand::handle(User* user, Server_msg* msg)
{
	msg->unknowncommand_msg(user, get_channel());
}

Err_InviteOnlyChan::Err_InviteOnlyChan(std::string channel)
{
	this->channel = channel;
}

void Err_InviteOnlyChan::handle(User* user, Server_msg* msg)
{
	msg->inviteonlychan_msg(user, get_channel());
}

Err_ChannelIsFull::Err_ChannelIsFull(std::string channel)
{
	this->channel = channel;
}

void Err_ChannelIsFull::handle(User* user, Server_msg* msg)
{
	msg->channelisfull_msg(user, get_channel());
}

Err_NoSuchChannel::Err_NoSuchChannel(std::string channel)
{
	this->channel = channel;
}

void Err_NoSuchChannel::handle(User* user, Server_msg* msg)
{
	msg->nosuchchannel_msg(user, get_channel());
}

Err_BadChannelKey::Err_BadChannelKey(std::string channel)
{
	this->channel = channel;
}

void Err_BadChannelKey::handle(User* user, Server_msg* msg)
{
	msg->badchannelkey_msg(user, get_channel());
}

Err_NoSuchNick::Err_NoSuchNick(std::string nick)
{
	this->nick = nick;
}

void Err_NoSuchNick::handle(User* user, Server_msg* msg)
{
	msg->nosuchnick_msg(user, get_channel());;
}

Err_CannotSendToChan::Err_CannotSendToChan(std::string channel)
{
	this->channel = channel;
}

void Err_CannotSendToChan::handle(User* user, Server_msg* msg)
{
	msg->cannotsendtochan_msg(user, get_channel());;
}

Err_NotOnChannel::Err_NotOnChannel(std::string channel)
{
	this->channel = channel;
}

void Err_NotOnChannel::handle(User* user, Server_msg* msg)
{
	msg->notonchannel_msg(user, get_channel());
}

Err_chanoprivsneeded::Err_chanoprivsneeded(std::string channel)
{
	this->channel = channel;
}

void Err_chanoprivsneeded::handle(User* user, Server_msg* msg)
{
	msg->err_chanoprivneeded_msg(user, get_channel());
}

Err_useronchannel::Err_useronchannel(std::string nick, std::string channel)
{
	this->channel = channel;
	this->nick = nick;
}

void Err_useronchannel::handle(User* user, Server_msg* msg)
{
	msg->err_useronchannel_msg(user, get_channel(), get_nick());
}

Err_UserNotInChannel::Err_UserNotInChannel(std::string nick, std::string channel)
{
	this->channel = channel;
	this->nick = nick;
}

void Err_UserNotInChannel::handle(User* user, Server_msg* msg)
{
	msg->err_usernotinchannel_msg(user, get_channel(), get_nick());
}

