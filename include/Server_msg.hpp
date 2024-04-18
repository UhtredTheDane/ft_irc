 
#ifndef SERVER_MSG_HPP
# define SERVER_MSG_HPP

# include <sys/socket.h>
# include "User.hpp"
# include "Channel.hpp"

class User;

class Server_msg{
	public:

			void welcome_msg(User* user);
			void yourhost_msg(User* user);
			void created_msg(User* user);
			void myinfo_msg(User* user);
			void whois_msg(User* user);
			void ping_msg(User* user);
			void pong_msg(User* user);
			void join_msg(User *user, Channel* channel);
			void mode_msg(User* user, Channel* channel);
			void leave_msg(User* user, Channel* channel);
			void alreadyregistred_msg(User* user);
			void passwordincorrect_msg(User* user);
			void nosuchchannel_msg(User* user, std::string& channel_name);
			void needmoreparams_msg(User* user, std::string& command);
			void notonchannel_msg(User* user, std::string& channel_name);
			void err_keyset_msg(User* user, std::string& channel_name);
			void err_chanoprivneeded_msg(User* user, std::string& channel_name);
			void err_usernotinchannel_msg(User* user, std::string& channel_name, std::string& nick);
			void err_unknowmode_msg(User* user,std::string& channel_name ,std::string & option);
			void err_nosuchnick_msg(User* user,std::string& nick);
			void err_useronchannel_msg(User* user,std::string& channel_name, std::string& nick);

	private:
};


#endif
