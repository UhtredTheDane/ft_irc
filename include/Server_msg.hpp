 
#ifndef SERVER_MSG_HPP
# define SERVER_MSG_HPP

# include <sys/socket.h>
# include "User.hpp"
# include "Channel.hpp"
# include <map>

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
			int chan_msg(User* user, Channel *curent_chan, std::vector<std::string> split_line);
			int priv_msg(User* user, std::vector<std::string> split_line, std::map<int, User*> users_map);
			void alreadyregistred_msg(User* user);
			void passwordincorrect_msg(User* user);
			void nosuchchannel_msg(User* user, std::string& channel_name);
			void needmoreparams_msg(User* user);
			void notonchannel_msg(User* user, std::string& channel_name);
			void nosuchnick_msg(User* user, std::string& user_name);
			void norecipient_msg(User* user, std::string& user_name);
			void notexttosend_msg(User* user, std::string& user_name);
			void cannotsendtochan_msg(User* user, std::string& channel_name);

	private:
};


#endif
