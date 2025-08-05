#include "../includes/Errors.hpp"

namespace Errors{
    std::string ERR_ALREADYREGISTERED(const Client& client){
        std::string str = client.getClient();
        str += " :You may not reregister";
        str += "\r\n";

        return str;
    }

    std::string ERR_BADCHANNELKEY(const Client& client, const Channel& chn){
        std::string str = client.getClient();
        str += ' ';
        str += chn.getName();
        str += " :Cannot join channel (+k)";
        str += "\r\n";

        return str;
    }

    std::string ERR_BANNEDFROMCHAN(const Client& client, const Channel& chn){
        std::string str = client.getClient();
        str += ' ';
        str += chn.getName();
        str += " :Cannot join channel (+b)";
        str += "\r\n";

        return str;
    }

    std::string ERR_CHANNELISFULL(const Client& client, const Channel& chn){
        std::string str = client.getClient();
        str += ' ';
        str += chn.getName();
        str += " :Cannot join channel (+l)";
        str += chn.getName();
        str += "\r\n";

        return str;
    }

    std::string ERR_CHANOPRIVSNEEDED(const Client& client, const Channel& chn){
        std::string str = client.getClient();
        str += ' ';
        str += chn.getName();
        str += " :You're not channel operator";
        str += "\r\n";

        return str;
    }

    std::string ERR_ERRONEUSNICKNAME(const Client& client){
        std::string str = client.getClient();
        str += " :Erroneus nickname";
        str += "\r\n";

        return str;
    }

    std::string ERR_INVITEONLYCHAN(const Client& client, const Channel& chn){
        std::string str = client.getClient();
        str += ' ';
        str += chn.getName();
        str += " :Cannot join channel (+i)";
        str += "\r\n";

        return str;
    }

    std::string ERR_NEEDMOREPARAMS(const Client& client, const Input& in){
        std::string str = client.getClient();
        str += ' ';
        str +=  in.getCommand();
        str +=  " :Not enough parameters";
        str += "\r\n";
        
        return str;
    }

        std::string ERR_NICKNAMEINUSE(const Client& client){
        std::string str = client.getClient();
        str += ' ';
        str += client.getNickname();
        str += " :Nickname is already in use";
        str += "\r\n";

        return str;
    }

    std::string ERR_NONICKNAMEGIVEN(const Client& client){
        std::string str = client.getClient();
        str += " :No nickname given";
        str += "\r\n";

        return str;
    }

    std::string ERR_NOSUCHCHANNEL(const Client& client, const Channel& chn){
        std::string str = client.getClient();
        str += ' ';
        str += chn.getName();
        str += " :No such channel";
        str += "\r\n";

        return str;
    }

    std::string ERR_NOTONCHANNEL(const Client& client, const Channel& chn){
        std::string str = client.getClient();
        str += ' ';
        str += chn.getName();
        str += " :You're not on that channel";
        str += "\r\n";

        return str;
    }

    std::string ERR_PASSWDMISMATCH(const Client& client){
        std::string str = client.getClient();
        str += " :Password incorrect";
        str += "\r\n";

        return str;
    }

    std::string ERR_USERNOTINCHANNEL(const Client& client, const Channel& chn){
        std::string str = client.getClient();
        str += ' ';
        str += client.getNickname();
        str +=  ' ';
        str += chn.getName();
        str += " :They aren't on that channel";
        str += "\r\n";

        return str;
    }

    std::string ERR_USERONCHANNEL(const Client& client, const Channel& chn){
        std::string str = client.getClient();
        str += ' ';
        str += client.getNickname();
        str += ' ';
        str += chn.getName();
        str += " :is already on channel";
        str += "\r\n";

        return str;
    }
}
