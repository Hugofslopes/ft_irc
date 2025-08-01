#ifndef REPLY_HPP
# define REPLY_HPP

# include "Client.hpp"
# include "Server.hpp"

class Server;
namespace Reply{
    std::string RPL_NOTOPIC(const Client& client, const Channel& ch);
    std::string RPL_TOPIC (const Client& client, const Channel& ch);
    std::string RPL_INVITING(const Client& client, const Channel& ch);
    std::string RPL_WELCOME(const Client& client, const Server& server);
    std::string RPL_YOURHOST(const Client& client, const Server& server);
    std::string RPL_CREATED(const Client& client, const Server& server);
}
#endif