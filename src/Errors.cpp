#include "../includes/Errors.hpp"

namespace Errors
{
	std::string	ERR_ALREADYREGISTERED(const Client& client)
	{
		std::string str = client.getNickname();
		str += " :You may not reregister";

		return (str);
	}

	std::string	ERR_BADCHANNELKEY(const Client& client, const Channel& chn)
	{
		std::string	str = client.getNickname();
		str += ' ';
		str += chn.getName();
		str += " :Cannot join channel (+k)";

		return (str);
	}

	std::string	ERR_BANNEDFROMCHAN(const Client& client, const Channel& chn)
	{
		std::string	str = client.getNickname();
		str += ' ';
		str += chn.getName();
		str += " :Cannot join channel (+b)";

		return (str);
	}

	std::string	ERR_CHANNELISFULL(const Client& client, const Channel& chn)
	{
		std::string	str = client.getNickname();
		str += ' ';
		str += chn.getName();
		str += " :Cannot join channel (+l)";
		str += chn.getName();

		return (str);
	}

	std::string	ERR_CHANOPRIVSNEEDED(const Client& client, const Channel& chn)
	{
		std::string	str = client.getNickname();
		str += ' ';
		str += chn.getName();
		str += " :You're not channel operator";

		return (str);
	}

	std::string	ERR_ERRONEUSNICKNAME(const Client& client)
	{
		std::string	str = client.getNickname();
		str += " :Erroneous nickname";

		return (str);
	}

	std::string	ERR_INVITEONLYCHAN(const Client& client, const Channel& chn)
	{
		std::string	str = client.getNickname();
		str += ' ';
		str += chn.getName();
		str += " :Cannot join channel (+i)";

		return (str);
	}

	std::string	ERR_NEEDMOREPARAMS(const Client& client, Input in)
	{
		std::string	str = client.getNickname();
		str += ' ';
		str +=  in.getFullInput();
		str +=  " :Not enough parameters";

		return (str);
	}

	std::string	ERR_NICKNAMEINUSE(const Client& client)
	{
		std::string str = client.getNickname();
		str += ' ';
		str += client.getNickname();
		str += " :Nickname is already in use";

		return (str);
	}

	std::string ERR_NONICKNAMEGIVEN(const Client& client)
	{
		std::string	str = client.getNickname();
		str += " :No nickname given";

		return (str);
    }

	std::string	ERR_NOSUCHCHANNEL(const Client& client, const std::string& chn)
	{
		std::string	str = client.getNickname();
		str += ' ';
		str += chn;
		str += " :No such channel";

		return (str);
	}

	std::string	ERR_NOTONCHANNEL(const Client& client, const Channel& chn)
	{
		std::string str = client.getNickname();
		str += ' ';
		str += chn.getName();
		str += " :You're not on that channel";

		return (str);
	}

	std::string	ERR_PASSWDMISMATCH(const Client& client)
	{
		std::string str = client.getNickname();
		str += " :Password incorrect";

		return (str);
	}

	std::string	ERR_USERNOTINCHANNEL(const Client& client, const Channel& chn)
	{
		std::string	str = client.getNickname();
		str +=  ' ';
		str += chn.getName();
		str += " :They aren't on that channel";

		return (str);
	}

	std::string	ERR_USERONCHANNEL(const Client& client, const Channel& chn)
	{
		std::string	str = client.getNickname();
		str += ' ';
		str += client.getNickname();
		str += ' ';
		str += chn.getName();
		str += " :is already on channel";

		return (str);
	}
}
