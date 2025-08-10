#ifndef REPLY_HPP
# define REPLY_HPP

# include "Client.hpp"
# include "Server.hpp"
class Client;

class Server;
namespace Reply{
    std::string RPL_CREATED(const Client& client, const Server& server);
    std::string RPL_INVITING(const Client& client, const Channel& ch);
    std::string RPL_JOIN(const Client& client, const Channel& ch);
    std::string RPL_NOTOPIC(const Client& client, const Channel& ch);
    std::string RPL_TOPIC (const Client& client, const Channel& ch);
    std::string RPL_WELCOME(const Client& client, const Server& server);
    std::string RPL_YOURHOST(const Client& client, const Server& server);
    std::string	RPL_MYINFO(const Client& client, const Server& server);
    std::string RPL_JOINEDCHA(const Client& client, const Channel& ch);
    std::string RPL_JOINEDCHATOPIC(const Server& sv, const Client& client, const Channel& ch);
    std::string RPL_NAMREPLY(const Client& client, const Channel& ch);
    std::string RPL_ENDOFNAMES(const Client& client, const Channel& ch);
    std::string RPL_WHO_REPLY(const Server& sv, const Channel& ch, const Client& cl);
    std::string RPL_WHO_END_REPLY(const Server& sv, const Channel& ch, const Client& cl);
}
#endif
