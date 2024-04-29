#ifndef IRCEXCEPTIONS_HPP
# define IRCEXCEPTIONS_HPP

# include "AException.hpp"

class Server_msg;

class Err_PasswordIncorrect : public AException
{
	public:
		void handle(User* user, Server_msg* msg);
};

class Err_AlreadyRegistred : public AException
{
	public:
		void handle(User* user, Server_msg* msg);
};
class Err_NotRegistred: public AException
{
	public:
		Err_NotRegistred(int socket);
		void handle(User* user, Server_msg* msg);
};
class Err_NeedMoreParams : public AException
{
	public:
		Err_NeedMoreParams(std::string channel);
		void handle(User* user, Server_msg* msg);
};
		
class Err_InviteOnlyChan : public AException
{
	public:
		Err_InviteOnlyChan(std::string channel);
		void handle(User* user, Server_msg* msg);	
};

class Err_ChannelIsFull : public AException
{
	public:
		Err_ChannelIsFull(std::string channel);
		void handle(User* user, Server_msg* msg);
};	

class Err_NoSuchChannel : public AException
{
	public:
		Err_NoSuchChannel(std::string str);
		void handle(User* user, Server_msg* msg);
};

class Err_BadChannelKey : public AException
{
	public:
		Err_BadChannelKey(std::string channel);
		void handle(User* user, Server_msg* msg);
};

class Err_NoSuchNick : public AException
{
	public:
		Err_NoSuchNick(std::string str);
		void handle(User* user, Server_msg* msg);
};

class Err_CannotSendToChan: public AException
{
	public:
		Err_CannotSendToChan(std::string str);
		void handle(User* user, Server_msg* msg);
};

class Err_NotOnChannel : public AException
{
	public:
		Err_NotOnChannel(std::string str);
		void handle(User* user, Server_msg* msg);
};

class Err_chanoprivsneeded : public AException
{
	public:
		Err_chanoprivsneeded(std::string channel);
		void handle(User* user, Server_msg* msg);
};

class Err_useronchannel : public AException
{
	public:
		Err_useronchannel(std::string nick, std::string channel);
		void handle(User* user, Server_msg* msg);
};

class Err_UserNotInChannel : public AException
{
	public:
		Err_UserNotInChannel(std::string nick, std::string channel);
		void handle(User* user, Server_msg* msg);
};
class Err_KeySet : public AException
{
	public:
		Err_KeySet(std::string channel);
		void handle(User* user, Server_msg* msg);
};
class Err_NoChanModes : public AException
{
	public:
		Err_NoChanModes(std::string channel);
		void handle(User* user, Server_msg* msg);
};
class Err_UnknowedMode : public AException
{
	public:
		Err_UnknowedMode(char c,std::string channel);
		void handle(User* user, Server_msg* msg);
		char getChar();
	private :
		char c;
};

#endif


