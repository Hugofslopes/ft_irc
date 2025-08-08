#include "../includes/Reply.hpp"
#include <string>

namespace Reply
{
	std::string	RPL_NOTOPIC(const Client& client, const Channel& ch)
	{
		std::string	str = client.getClient();
		str += ' ';
		str += ch.getName();
		str += " :No topic is set\r\n";

		return (str);
	}

	std::string	RPL_TOPIC(const Client& client, const Channel& ch)
	{
		std::string str = client.getClient();
		str += ' ';
		str += ch.getName();
		str += " :";
		str += ch.getTopic();
		str += "\r\n";

		return (str);
	}

	std::string	RPL_INVITING(const Client& client, const Channel& ch)
	{
		std::string	str = client.getClient();
		str += ' ';
		str += client.getNickname() ;
		str += ' ';
		str += ch.getName();
		str += "\r\n";

		return (str);
	}

	std::string	RPL_WELCOME(const Client& client, const Server& server)
	{
		std::string	str = client.getClient();
		str += " :Welcome to the ";
		str += server.getName();
		str += " Network, ";
		str += client.getNickname() ;
		str += "\r\n";

		return (str);
	}

	std::string	RPL_YOURHOST(const Client& client, const Server& server)
	{
		std::string	str = client.getClient();
		str += " :Your host is ";
		str += server.getName();
		str += ", running version ";
		str += server.getVersion();
		str += "\r\n";

		return (str);
	}

	std::string	RPL_CREATED(const Client& client, const Server& server)
	{
		std::string	str = client.getClient();
		str += " :This server was created ";
		str += server.getStTime();
		str += "\r\n";

		return str;
	}

    std::string RPL_JOIN(const Client& client, const Channel& ch){
        std::string str = 
        (":" + client.getNickname() + "!user@host JOIN " + ch.getName() + "\r\n" +
        "Now talking on #" +  ch.getName() + "\r\n" +
        "Topic for #" + ch.getName() + " is: " + ch.getTopic() + "\r\n");
        return str;
    }

    std::string RPL_JOINEDCHA(const Client& client){
        std::string str = client.getClient();
        str += " (";
        str += client.getClient();
        str += "@127.0.0.1) has joined";
        str += "\r\n";
        return str;
    }
}
