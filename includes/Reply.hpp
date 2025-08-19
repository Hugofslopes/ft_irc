#ifndef REPLY_HPP
# define REPLY_HPP

# include "Client.hpp"
# include "Server.hpp"
class Client;

class Server;
namespace Reply{
    std::string RPL_CREATED(const Server&);
    std::string RPL_INVITING(const Client&, const Channel&, const std::string&, const Server&);
    std::string RPL_NOTOPIC(const Client&, const Channel&);
    std::string	RPL_TOPIC(const Client&, const Channel&, const std::string);
    std::string RPL_WELCOME(const Client&, const Server&);
    std::string RPL_YOURHOST(const Server&);
    std::string	RPL_MYINFO(const Client&, const Server&);
    std::string RPL_JOINEDCHA(const Client&, const Channel&);
    std::string RPL_JOINEDCHATOPIC(const Server&, const Client&, const Channel&);
    std::string RPL_NAMREPLY(const Client&, const Channel&);
    std::string RPL_ENDOFNAMES(const Client&, const Channel&);
    std::string RPL_WHO_REPLY(const Server&, const Channel&, const Client&);
    std::string RPL_WHO_END_REPLY(const Server&, const Channel&, const Client&);
    std::string RPL_PART(const Client&, const Channel&);
    std::string RPL_QUIT(const Client&, const std::string&);
    std::string RPL_MODE(const Server& sv, const Client& cl, const Channel& ch);
    std::string RPL_LISTSTART(const Client& client);
    std::string RPL_LIST(const Client& client, const Channel& ch);
    std::string RPL_LISTEND(const Client& client);
    
}
#endif
