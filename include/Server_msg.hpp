
#ifndef SERVER_MSG_HPP
# define SERVER_MSG_HPP

# include <sys/socket.h>
# include "User.hpp"
# include "Channel.hpp"
class User;

class Server_msg{
	public:


			void part_request(User* user);
			void privmsg_request(User* user);
			void kick_request(User* user);
			void mode_request(User* user);
			void join_request(User* user);
			void pong_request(User* user);




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
	private:
		std::string request_types[6];
		void (Server_msg::*requests_ptr[6])(User*);
};


#endif
