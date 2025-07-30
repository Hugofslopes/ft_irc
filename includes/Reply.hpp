#ifndef REPLY_HPP
#define REPLY_HPP

#include <iostream>

#include "Client.hpp"
#include "Input.hpp"
#include "Channel.hpp"
#include "Server.hpp"

namespace REPLY{
    void RPL_NOTOPIC(const Client& client, const Channel& ch);
    void RPL_TOPIC (const Client& client, const Channel& ch);
    void RPL_INVITING(const Client& client, const Channel& ch);
    void RPL_WELCOME(const Client& client, const Server& server);
    void RPL_YOURHOST(const Client& client, const Server& server);
    void RPL_CREATED(const Client& client, const Server& server);
}
#endif