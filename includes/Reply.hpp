#ifndef REPLY_HPP
#define REPLY_HPP

#include <iostream>

#include "Client.hpp"
#include "Input.hpp"
#include "Channel.hpp"
#include "Irc.hpp"

namespace REPLY{
    void RPL_NOTOPIC(const Client& client, const Channel& ch);
    void RPL_TOPIC (const Client& client, const Channel& ch);
    void RPL_INVITING(const Client& client, const Channel& ch);
    void RPL_WELCOME(const Client& client);
    void RPL_YOURHOST(const Client& client);
}
#endif