
#ifndef SERVER_MSG_HPP
# define SERVER_MSG_HPP

# include "User.hpp"
class User;

class Server_msg{
	public:

			std::string welcome_msg(User user);
			std::string yourhost_msg(User user);
			std::string created_msg(User user);
			std::string myinfo_msg(User user);
			std::string whois_msg(User user);
			std::string ping_msg(User user);
			std::string pong_msg(User user);



	private:
};


#endif