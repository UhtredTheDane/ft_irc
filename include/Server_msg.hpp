
#ifndef SERVER_MSG_HPP
# define SERVER_MSG_HPP

# include <sys/socket.h>
# include "User.hpp"
# include "Channel.hpp"
# include <map>
# include <vector>

class User;

class Server_msg{
	public:

			std::string get_request_type(size_t i);
			std::string get_request_ptr(size_t i);
			void processing_request(int client_socket, User* user, std::string& request);
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
		std::map<std::string, Channel*> channels;
		std::vector<std::string> split_line;
};


#endif
