#include "../includes/Reply.hpp"
#include <string>

namespace Reply
{
	std::string	RPL_NOTOPIC(const Client& client, const Channel& ch)
	{
		std::string	str = client.getNickname();
		str += ' ';
		str += ch.getName();
		str += " :No topic is set";

		return (str);
	}

	std::string	RPL_TOPIC(const Client& client, const Channel& ch, const std::string topic)
	{
		std::string str = ":";
		str += client.getNickname();
		str += '!';
		str += client.getUsername();
		str += "@localhost TOPIC " ;
		str += ch.getName();
		str += ' ';
		str += topic;

		return (str);
	}

	std::string RPL_INVITING(const Client& client, const Channel& ch, const std::string& targetNick, 
		const Server& serverName) {
		std::string str = ":" + serverName.getName() + " 341 ";
		str += client.getNickname();
		str += " ";
		str += targetNick;
		str += " ";
		str += ch.getName();

		return str;
	}

	std::string	RPL_WELCOME(const Client& client, const Server& server)
	{
		std::string	str = "001 ";
		str += client.getNickname();
		str += " :Welcome to ";
		str += server.getName();
		str += " network, ";
		str += client.getNickname() ;

		return (str);
	}

	std::string	RPL_YOURHOST(const Server& server)
	{
		std::string	str =  "002 :Your host is ";
		str += server.getName();
		str += ", running version ";
		str += server.getVersion();
		return (str);
	}

	std::string	RPL_CREATED(const Server& server)
	{
		std::string	str = "003 :This server was created ";
		str += server.getStTime();

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
		return str;
    }

	std::string RPL_JOINEDCHATOPIC(const Server& sv, const Client& client, const Channel& ch){
		std::string str = ":" + sv.getName() + " 332 ";
		str += client.getNickname() + " " + ch.getName() + " :" + ch.getTopic();
		return str;
	}

	std::string RPL_NAMREPLY(const Client& client, const Channel& ch) {
		std::string str = ":";
		str += client.getNickname() + "!" + client.getUsername() + "@localhost";
		str += " 353 ";
		str += client.getNickname();
		str += " = ";
		str += ch.getName();
		str += " :";

		const std::vector<std::string>& members = ch.getMembers();
		const std::vector<std::string>& operators = ch.getOperators();

		for (std::vector<std::string>::const_iterator it = members.begin(); it != members.end(); ++it) {
			std::string prefix = "";
			if (std::find(operators.begin(), operators.end(), *it) != operators.end())
				prefix = "@";
			str += prefix + *it + " ";
		}

		return str;
	}

	std::string RPL_ENDOFNAMES(const Client& client, const Channel& ch) {
		std::string str = ":";
		str += client.getNickname() + "!" + client.getUsername() + "@" + "localhost";
		str += " 366 ";
		str += client.getNickname();
		str += " ";
		str += ch.getName();
		str += " :End of /NAMES list.";
    return str;
	}

	std::string RPL_WHO_REPLY(const Server& sv, const Channel& ch, const Client& cl) {
		std::vector<std::string> operators = ch.getOperators();
		std::vector<std::string>::const_iterator it = operators.begin();
		bool status = false;
		std::string statusS = "H";

		for (; it != operators.end(); it++)
			if (*it == cl.getNickname())
				status = true;

		if (status)
			statusS += "@";

		return ":" + sv.getName() + " 352 " + cl.getNickname() + " " + ch.getName() + " " +
		cl.getUsername() + " localhost " + sv.getName() + " " + cl.getNickname() + " " +
		statusS + " :0 " + cl.getClient();
	}

	std::string RPL_WHO_END_REPLY(const Server& sv, const Channel& ch, const Client& cl){
		return ":" + sv.getName() + " 315 " + cl.getNickname() + " " + ch.getName() + 
		" :End of /WHO list.";
	}

	std::string RPL_PART(const Client& client, const Channel& channel) {
    	std::string msg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + "localhost";
    	msg += " PART " + channel.getName();
    	return msg;
	}

	std::string RPL_QUIT(const Client& client, const std::string& quitMessage) {
		std::string msg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost";
		msg += " QUIT :" + quitMessage;
    return msg;
	}

	std::string RPL_MODE(const Server& sv, const Client& cl, const Channel& ch) {
		std::string msg = ":" + sv.getName() + " 324 " + cl.getNickname() + ' ' + 
		ch.getName() + ' ';
		if (ch.getInvite())
		{
			msg += "+i";
			
		}
		if (ch.getKey())
		{
			if (msg[msg.size() - 1] == ' ')
				msg += '+';
			msg += "k";
		}
		if (ch.getTopicRestricted())
		{
			if (msg[msg.size() - 1] == ' ')
				msg += '+';
			msg += "t";
		}
		if (ch.getUserLimit() != 0)
		{
			if (msg[msg.size() - 1] == ' ')
				msg += '+';
			msg += "l";
		}

    	return msg;
	}

	std::string RPL_LISTSTART(const Client& client) {
    return ":localhost 321 " + client.getNickname() + " Channel :Users Name";
	}

	std::string RPL_LIST(const Client& client, const Channel& ch) {
		std::string str = ":localhost 322 ";
		str += client.getNickname() + " ";
		str += ch.getName() + " ";
		std::ostringstream oss;
		oss << ch.getMemberCount();
		str += oss.str() + " ";
		str += ":" + ch.getTopic();
		std::cout << str << std::endl;	
    	return str;
	}

	std::string RPL_LISTEND(const Client& client) {
		return ":localhost 323 " + client.getNickname() + " :End of /LIST";
	}
}
