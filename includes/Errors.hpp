#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <iostream>

#include "Client.hpp"
#include "Input.hpp"
#include "Channel.hpp"

namespace Errors{
    void ERR_NEEDMOREPARAMS(const Client&, const Input&);
    void ERR_ALREADYREGISTERED(const Client&);
    void ERR_PASSWDMISMATCH(const Client&);
    void ERR_NONICKNAMEGIVEN(const Client&);
    void ERR_ERRONEUSNICKNAME(const Client&);
    void ERR_NICKNAMEINUSE(const Client&);
    void ERR_BADCHANNELKEY(const Client& client, const Channel& chn);
    void ERR_BANNEDFROMCHAN(const Client& client, const Channel& chn);
    void ERR_CHANNELISFULL(const Client& client, const Channel& chn);
    void ERR_INVITEONLYCHAN(const Client& client, const Channel& chn);
    void ERR_NOSUCHCHANNEL(const Client& client, const Channel& chn);
    void ERR_NOTONCHANNEL(const Client& client, const Channel& chn);
    void ERR_CHANOPRIVSNEEDED(const Client& client, const Channel& chn);
    void ERR_USERONCHANNEL(const Client& client, const Channel& chn);
    void ERR_USERNOTINCHANNEL(const Client& client, const Channel& chn);
}
#endif  