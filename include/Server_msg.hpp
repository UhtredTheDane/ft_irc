
#ifndef SERVER_MSG_HPP
# define SERVER_MSG_HPP

#include <sys/socket.h>
# include "User.hpp"
class User;

class Server_msg{
	public:

			void welcome_msg(User* user, int client_socket);
			void yourhost_msg(User* user, int client_socket);
			void created_msg(User* user, int client_socket);
			void myinfo_msg(User* user, int client_socket);
			void whois_msg(User* user, int client_socket);
			void ping_msg(int client_socket);
			void pong_msg(int client_socket);
	private:
};


#endif
