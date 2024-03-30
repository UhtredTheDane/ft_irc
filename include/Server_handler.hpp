#ifndef SERVER_HANDLER_HPP
# define SERVER_HANDLER_HPP

# include "User.hpp"
# include "Channel.hpp"
# include "Serveur.hpp"
# include "Server_msg.hpp"
# include "vector.hpp"
# include "Message.hpp"

class Server_handler{
	public:
        Server_handler(Server& serv);
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
        Serveur* serv;
        Server_msg msg;
        std::vector<std::string> split_line;
        std::string request_types[6];
		void (Server::*requests_ptr[6])(User*);

};

#endif
