#include "../include/AException.hpp"

int AException::get_socket(void)
{
    return (socket);
}

std::string AException::get_channel(void)
{
    return (channel);
}

std::string AException::get_nick(void)
{
    return (nick);
}

std::string AException::get_cmd(void)
{
    return (cmd);
}