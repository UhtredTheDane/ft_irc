 
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
			void notregistred_msg(int socket);
			void passwordincorrect_msg(User* user);
			void nosuchchannel_msg(User* user, std::string& channel_name);
			void needmoreparams_msg(User* user, std::string& command);
			void notonchannel_msg(User* user, std::string& channel_name);
			void print_send(int client_socket, std::string msg, int length, int param);
			void inviteonlychan_msg(User* user, std::string& channel);
			void channelisfull_msg(User* user, std::string& channel);
			void badchannelkey_msg(User* user, std::string& channel);
			void err_keyset_msg(User* user, std::string& channel_name);
			void err_chanoprivneeded_msg(User* user, std::string const& channel_name);
			void err_usernotinchannel_msg(User* user, std::string& channel_name, std::string& nick);
			void err_unknowmode_msg(User* user,std::string& channel_name ,std::string & option);
			void err_nosuchnick_msg(User* user,std::string& nick);
			void err_useronchannel_msg(User* user,std::string const& channel_name, std::string const& nick);
			void cannotsendtochan_msg(User* user, std::string& channel_name);
			int  kick_msg(User* user, Channel *curent_chan, std::vector<std::string> split_line);
			void nosuchnick_msg(User* user, std::string& user_name);
			void norecipient_msg(User* user, std::string& user_name);
			void notexttosend_msg(User* user, std::string& user_name);
			void nicknameinuse_msg(User* user, std::string oldnick,  std::string nick);

	private:
};


#endif
