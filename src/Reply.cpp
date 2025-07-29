#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"
#include "../includes/Reply.hpp"
#include "../includes/Irc.hpp"


namespace REPLY{
    void RPL_NOTOPIC(const Client& client, const Channel& ch){
        std::cerr << client.getClient() << ' ' << ch.getName() 
        << " :No topic is set" << std::endl;
    }

    void RPL_TOPIC(const Client& client, const Channel& ch){
        std::cerr << client.getClient() << ' ' << ch.getName() 
        << " :" << ch.getTopic() << std::endl;
    }

    void RPL_INVITING(const Client& client, const Channel& ch){
        std::cerr << client.getClient() << ' ' << client.getNick() 
        << ' ' << ch.getName() << std::endl;
    }

    void RPL_WELCOME(const Client& client){
        std::cerr << client.getClient() << " :Welcome to the " << Irc::network_name 
        << " Network, " << client.getNick() << std::endl;
    }

    void RPL_YOURHOST(const Client& client){
        std::cerr << client.getClient() << " :Your host is " << Irc::network_name 
        << ", running version " << Irc::version << std::endl;
    }

    void RPL_CREATED(const Client& client){
        std::cerr << client.getClient() << " :This server was created " << 
        Irc::getDateTime() << std::endl;
    }

}