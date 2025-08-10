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
		std::string	str = "001 ";
		str += client.getNickname();
		str += " :Welcome to ";
		str += server.getName();
		str += " network, ";
		str += client.getNickname() ;
		str += "\r\n";

		return (str);
	}

	std::string	RPL_YOURHOST(const Client& client, const Server& server)
	{
		std::string	str =  "002 :Your host is ";
		str += server.getName();
		str += ", running version ";
		str += server.getVersion();
		str += "\r\n";
		(void)client;
		return (str);
	}

	std::string	RPL_CREATED(const Client& client, const Server& server)
	{
		std::string	str = "003 :This server was created ";
		str += server.getStTime();
		str += "\r\n";
		(void)client;

		return str;
	}

	std::string	RPL_MYINFO(const Client& client, const Server& server)
	{
		std::string	str =server.getName();
		str += " 004 ";
		str += client.getNickname();
		str += ' ';
		str += server.getVersion();
		str += " :Available user modes: o, channel modes: tkl";
		str += "\r\n";

		return str;
	}

    std::string RPL_JOIN(const Client& client, const Channel& ch){
        std::string str = 
        (":" + client.getNickname() + "!" + client.getUsername() + "@localhost JOIN " + ch.getName() + "\r\n" +
        "Now talking on #" +  ch.getName() + "\r\n" +
        "Topic for #" + ch.getName() + " is: " + ch.getTopic() + "\r\n");
        return str;
    }

    std::string RPL_JOINEDCHA(const Client& client, const Channel& ch){
        std::string str = ":";
		str += client.getNickname();
		str += "!";
		str += client.getUsername();
		str += "@";
		str += "localhost";
		str += " JOIN ";
		str += ch.getName();
		str += "\r\n";
		return str;
    }

	std::string RPL_JOINEDCHATOPIC(const Server& sv, const Client& client, const Channel& ch){
		std::string str = ":" + sv.getName() + " 332 ";
		str += client.getNickname() + " " + ch.getName() + " :" + ch.getTopic() + "\r\n";
		return str;
	}

	
	std::string RPL_NAMREPLY(const Client& client, const Channel& ch) {
		std::string str = ":";
		str += client.getNickname() + "!" + client.getUsername() + "@" + "localhost";
		str += " 353 ";
		str += client.getNickname();
		str += " = ";
		str += ch.getName();
		str += " :";

		std::vector<std::string>::const_iterator it = ch.getMembers().begin();
		for (; it != ch.getMembers().end(); it++)
			str += *it + " ";
		str += "\r\n";
		return str;
	}

	std::string RPL_ENDOFNAMES(const Client& client, const Channel& ch) {
		std::string str = ":";
		str += client.getNickname() + "!" + client.getUsername() + "@" + "localhost";
		str += " 366 ";
		str += client.getNickname();
		str += " ";
		str += ch.getName();
		str += " :End of /NAMES list.\r\n";
    return str;
	}

	std::string RPL_WHO_REPLY(const Server& sv, const Channel& ch, const Client& cl) {
		std::vector<std::string>::const_iterator it = ch.getOperators().begin();
		bool status = false;
		std::string statusS = "H";

		for (; it != ch.getMembers().end(); it++)
			if (*it == cl.getNickname())
				status = true;

		if (status)
			statusS += "@";

		return ":" + sv.getName() + " 352 " + cl.getNickname() + " " + ch.getName() + " " +
		cl.getUsername() + " lochalhost " + sv.getName() + " " + cl.getNickname() + " " +
		statusS + " :" + '0' + " " + cl.getClient();
	}

	std::string RPL_WHO_END_REPLY(const Server& sv, const Channel& ch, const Client& cl){
		return ":" + sv.getName() + " 315 " + cl.getNickname() + " " + ch.getName() + 
		" :End of /WHO list.";
	}

}

