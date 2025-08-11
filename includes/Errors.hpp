#ifndef ERRORS_HPP
# define ERRORS_HPP

# include "Client.hpp"
# include "Input.hpp"
# include "Channel.hpp"
# include <string>

class Client;

namespace Errors
{
	std::string	ERR_ALREADYREGISTERED(const Client&);
	std::string	ERR_BADCHANNELKEY(const Client&, const Channel&);
	std::string	ERR_BANNEDFROMCHAN(const Client&, const Channel&);
	std::string	ERR_CHANNELISFULL(const Client&, const Channel&);
	std::string	ERR_CHANOPRIVSNEEDED(const Client&, const Channel&);
	std::string	ERR_ERRONEUSNICKNAME(const Client&);
	std::string	ERR_INVITEONLYCHAN(const Client&, const Channel&);
	std::string	ERR_NEEDMOREPARAMS(const Client&, Input);
	std::string	ERR_NICKNAMEINUSE(const Client&);
	std::string	ERR_NONICKNAMEGIVEN(const Client&);
	std::string	ERR_NOSUCHCHANNEL(const Client&, const std::string&);
	std::string	ERR_NOTONCHANNEL(const Client&, const Channel&);
	std::string	ERR_PASSWDMISMATCH(const Client&);
	std::string	ERR_USERNOTINCHANNEL(const Client&, const Channel&);
	std::string	ERR_USERONCHANNEL(const Client&, const Channel&);
}
#endif
