#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"
#include "../includes/Errors.hpp"
#include "../includes/Input.hpp"

namespace Errors{
    void ERR_NEEDMOREPARAMS(const Client& client, const Input& in){
        std::cerr << client.getClient() << in.getCommand() 
        << " :Not enough parameters"  << std::endl;
    }

    void ERR_ALREADYREGISTERED(const Client& client){
        std::cerr << client.getClient() << " :You may not reregister" << std::endl;
    }

    void ERR_PASSWDMISMATCH(const Client& client){
        std::cerr << client.getClient() << " :Password incorrect" << std::endl;
    }

    void ERR_NONICKNAMEGIVEN(const Client& client){
        std::cerr << client.getClient() << " :No nickname given" << std::endl;
    }

    void ERR_ERRONEUSNICKNAME(const Client& client){
        std::cerr << client.getClient() << " :Erroneus nickname" << std::endl;
    }

    void ERR_NICKNAMEINUSE(const Client& client){
        std::cerr << client.getClient() << ' ' <<  client.getNick() <<
        " :Nickname is already in use" << std::endl;
    }

    void ERR_BADCHANNELKEY(const Client& client, const Channel& chn){
        std::cerr << client.getClient() << ' ' <<  chn.getName() <<
        " :Cannot join channel (+k)" << std::endl;
    }

    void ERR_BANNEDFROMCHAN(const Client& client, const Channel& chn){
        std::cerr << client.getClient() << ' ' <<  chn.getName() <<
        " :Cannot join channel (+b)" << std::endl;
    }

    void ERR_CHANNELISFULL(const Client& client, const Channel& chn){
        std::cerr << client.getClient() << ' ' <<  chn.getName() <<
        " :Cannot join channel (+l)" << std::endl;
    }

    void ERR_INVITEONLYCHAN(const Client& client, const Channel& chn){
        std::cerr << client.getClient() << ' ' <<  chn.getName() <<
        " :Cannot join channel (+i)" << std::endl;
    }

    void ERR_NOSUCHCHANNEL(const Client& client, const Channel& chn){
        std::cerr << client.getClient() << ' ' <<  chn.getName() <<
        " :No such channel" << std::endl;
    }

    void ERR_NOTONCHANNEL(const Client& client, const Channel& chn){
        std::cerr << client.getClient() << ' ' <<  chn.getName() <<
        " :You're not on that channel" << std::endl;
    }

    void ERR_CHANOPRIVSNEEDED(const Client& client, const Channel& chn){
        std::cerr << client.getClient() << ' ' <<  chn.getName() <<
        " :You're not channel operator" << std::endl;
    }

    void ERR_USERONCHANNEL(const Client& client, const Channel& chn){
        std::cerr << client.getClient() << ' ' << client.getNick() << ' '
        << chn.getName() << " :is already on channel" << std::endl;
    }

    void ERR_USERNOTINCHANNEL(const Client& client, const Channel& chn){
        std::cerr << client.getClient() << ' ' << client.getNick() << ' '
        << chn.getName() << " :They aren't on that channel" << std::endl;
    }
}
