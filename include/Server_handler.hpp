#ifndef SERVER_HANDLER_HPP
# define SERVER_HANDLER_HPP

# include "User.hpp"
# include "Channel.hpp"
# include "Server.hpp"
# include "Server_msg.hpp"
# include <vector>
# include "Message.hpp"

class Server;

class Server_handler
{

	public:

        Server_handler(void);
		User* add_user(int fd_client);
		Channel* add_channel(std::string name, User* user);
        void request_handler(int client_socket, std::string &request);
		bool is_on_serv(std::string& nickname);
		void processing_request(int client_socket, User* user, std::string& request);
		void part_request(User* user);
		void privmsg_request(User* user);
		void kick_request(User* user);
		void mode_request(User* user);
		void join_request(User* user);
		void pong_request(User* user);

	private:
	
        Server_msg msg;
        std::vector<std::string> split_line;
        std::string request_types[10];
		void (Server_handler::*requests_ptr[10])(User*);
		std::map<int, User*> users_map;
		std::map<std::string, Channel*> channels;

};

#endif
