#ifndef AEXCEPTION_HPP
# define AEXCEPTION_HPP

# include <exception>

class AException : std::exception
{
	public:
        int get_socket(void);
        std::string get_channel(void);
        std::string get_nick(void);
        std::string get_cmd(void);
    	virtual void handle(User* user, Server_msg* msg) = 0;

    private:
        int socket = 0;
        std::string channel = "";
        std::string nick = "";
        std::string cmd = "";

};
#endif