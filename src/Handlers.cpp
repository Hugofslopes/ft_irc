#include "../includes/Server.hpp"
#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include "../includes/Reply.hpp"
#include "../includes/Errors.hpp"

void	Server::handleInvite()
{
	Client*	client = findClientByFd(_fds[_nbClients - 1].fd);

	if (!client || !client->isRegistered())
	{
		std::string	errMsg = ":" + _network_name + "451: You have not registered";
		sendMessage(_fds[_nbClients - 1].fd, errMsg);
		return ;
	}

	std::vector<std::string>	args = _input.getArgs();
	if (args.size() < 2)
	{
		sendMessage(client->getFd(), Errors::ERR_NEEDMOREPARAMS(*client, _input));
		return ;
	}

	std::string	targetNick = args[0];
	std::string	channelName = args[1];
	if (channelName[0] != '#')
		channelName = "#" + channelName;

	Channel*	channel = findChannel(channelName);
	if (!channel)
	{
		sendMessage(client->getFd(), Errors::ERR_NOSUCHCHANNEL(*client, *channel));
		return ;
	}
	if (!channel->isMember(client->getNickname()))
	{
		sendMessage(client->getFd(), Errors::ERR_NOTONCHANNEL(*client, *channel));
		return ;
	}
	if (!channel->isOperator(client->getNickname()))
	{
		sendMessage(client->getFd(), Errors::ERR_CHANOPRIVSNEEDED(*client, *channel));
		return ;
	}

	Client*	target = findClientByNick(targetNick);
	if (!target)
	{
		std::string	errMsg = ":" + _network_name + " 401 " + client->getNickname() + " "
			+ targetNick + ": No such nick/channel";
		sendMessage(client->getFd(), errMsg);
		return ;
	}
	
	channel->addInvited(targetNick);
	sendMessage(client->getFd(), Reply::RPL_INVITING(*client, *channel));
	std::string	inviteMsg = ":" + client->getNickname() + " INVITE " + targetNick + " " + channelName;
	sendMessage(client->getFd(), inviteMsg);
}

void	Server::handleJoin()
{
	Client*	client = findClientByFd(_fds[_nbClients - 1].fd);

	if (!client || !client->isRegistered())
	{
		std::string	errMsg = ":" + _network_name + " 451: You have not registered!";
		sendMessage(_fds[_nbClients - 1].fd, errMsg);
		return ;
	}

	std::vector<std::string>	args = _input.getArgs();
	if (args.empty())
	{
		sendMessage(client->getFd(), Errors::ERR_NEEDMOREPARAMS(*client, _input));
		return ;
	}

	std::string	channelName = args[0];
	if (channelName[0] != '#')
		channelName = "#" + channelName;
	std::string	key = args.size() > 1 ? args[1] : "";

	Channel*	channel = findChannel(channelName);
	if (!channel)
	{
		_channels[channelName] = Channel(channelName);
		channel = findChannel(channelName);
		channel->addMember(client->getNickname());
		channel->addOperator(client->getNickname()); //the creator is the default operator
	}
	else
	{
		if (channel->getUserLimit() > 0 && static_cast<int>(channel->getMembers().size()) >= channel->getUserLimit())
		{
			sendMessage(client->getFd(), Errors::ERR_CHANNELISFULL(*client, *channel));
			return ;
		}
		if (channel->getInvite() && channel->isInvited(client->getNickname()))
		{
			sendMessage(client->getFd(), Errors::ERR_INVITEONLYCHAN(*client, *channel));
			return ;
		}
		if (channel->getKey() && key != channel->getKeyValue())
		{
			sendMessage(client->getFd(), Errors::ERR_BADCHANNELKEY(*client, *channel));
			return ;
		}
		if (!channel->addMember(client->getNickname()))
		{
			sendMessage(client->getFd(), Errors::ERR_USERONCHANNEL(*client, *channel));
			return ;
		}
	}

	client->addChannel(channelName);
	std::string	joinMsg = ":" + client->getNickname() + " JOIN " + channelName;

	const std::vector<std::string>&	members = channel->getMembers();
	for (size_t i = 0; i < members.size(); ++i)
	{
		Client*	member = findClientByNick(members[i]);
		if (member)
			sendMessage(member->getFd(), joinMsg);
	}

	if (channel->getTopic().empty())
		sendMessage(client->getFd(), Reply::RPL_NOTOPIC(*client, *channel));
	else
		sendMessage(client->getFd(), Reply::RPL_TOPIC(*client, *channel));
}

void	Server::handleKick()
{
	Client*	client = findClientByFd(_fds[_nbClients - 1].fd);

	if (!client || !client->isRegistered())
	{
		std::string	errMsg = ":" + _network_name + " 451: You have not registered!";
		sendMessage(_fds[_nbClients - 1].fd, errMsg);
		return ;
	}

	std::vector<std::string>	args = _input.getArgs();
	if (args.size() < 2)
	{
		sendMessage(client->getFd(), Errors::ERR_NEEDMOREPARAMS(*client, _input));
		return ;
	}

	std::string	channelName = args[0];
	if (channelName[0] != '#')
		channelName = "#" + channelName;
	std::string	targetNick = args[1];
	std::string	reason = args.size() > 2 ? args[2] : "No reason";

	Channel*	channel = findChannel(channelName);
	if (!channel)
	{
		sendMessage(client->getFd(), Errors::ERR_NOSUCHCHANNEL(*client, *channel));
		return ;
	}
	if (!channel->isMember(client->getNickname()))
	{
		sendMessage(client->getFd(), Errors::ERR_NOTONCHANNEL(*client, *channel));
		return ;
	}
	if (!channel->isOperator(client->getNickname()))
	{
		sendMessage(client->getFd(), Errors::ERR_CHANOPRIVSNEEDED(*client, *channel));
		return ;
	}
	if (!channel->isMember(targetNick))
	{
		sendMessage(client->getFd(), Errors::ERR_USERNOTINCHANNEL(*client, *channel));
		return ;
	}

	channel->removeMember(targetNick);
	Client*	target = findClientByNick(targetNick);
	if (target)
	{
		target->removeChannel(channelName);
		std::string	kickMsg = ":" + client->getNickname() + " KICK " + channelName + " "
			+ targetNick + " :" + reason;
		const std::vector<std::string>&	members = channel->getMembers();
		for (size_t i = 0; i < members.size(); ++i)
		{
			Client*	member = findClientByNick(members[i]);
			if (member)
				sendMessage(member->getFd(), kickMsg);
		}
		sendMessage(target->getFd(), kickMsg);
	}
}

void	Server::handleMode()
{
	Client*	client = findClientByFd(_fds[_nbClients - 1].fd);
	if (!client || !client->isRegistered())
	{
		std::string	errMsg = ":" + _network_name + " 451 :You have not registered";
		sendMessage(_fds[_nbClients - 1].fd, errMsg);
		return ;
	}

	std::vector<std::string>	args = _input.getArgs();
	if (args.empty())
	{
		sendMessage(client->getFd(), Errors::ERR_NEEDMOREPARAMS(*client, _input));
		return ;
	}

	std::string	channelName = args[0];
	if (channelName[0] != '#')
		channelName = "#" + channelName;
	std::string	modeStr = args.size() > 1 ? args[1] : "";

	Channel*	channel = findChannel(channelName);
	if (!channel)
	{
		sendMessage(client->getFd(), Errors::ERR_NOSUCHCHANNEL(*client, *channel));
		return ;
	}
	if (!channel->isOperator(client->getNickname()))
	{
		sendMessage(client->getFd(), Errors::ERR_CHANOPRIVSNEEDED(*client, *channel));
		return ;
	}

	if (modeStr.empty())
	{
		std::string	modes = "+";
		if (channel->getInvite())
			modes += "i";
		if (channel->getTopicRestricted())
			modes += "t";
		if (channel->getKey())
			modes += "k";
		if (channel->getUserLimit() > 0)
			modes += "l";
		std::string	modeMsg = ":" + _network_name + " 324 " + client->getNickname()
			+ " " + channelName + " " + modes;
		sendMessage(client->getFd(), modeMsg);
		return ;
	}

	bool	adding = true;
	size_t	paramIndx = 2;
	for (size_t i = 0; i < modeStr.length(); ++i)
	{
		char	mode = modeStr[i];
		if (mode == '+')
		{
			adding = true;
			continue ;
		}
		if (mode == '-')
		{
			adding = false;
			continue ;
		}

		std::string	param = paramIndx < args.size() ? args[paramIndx++] : "";
		if (mode == 'i' && adding == true)
			channel->setInvite(adding);
		else if (mode == 't')
			channel->setTopicRestricted(adding);
		else if (mode == 'k')
		{
			if (adding && !param.empty())
				channel->setKey(param);
			else if (!adding)
				channel->removeKey();
		}
		else if (mode == 'o')
		{
			if (!param.empty())
			{
				if (!channel->isMember(param))
				{
					sendMessage(client->getFd(), Errors::ERR_USERNOTINCHANNEL(*client, *channel));
					return ;
				}
				if (adding)
				{
					channel->addOperator(param);
					Client*	target = findClientByNick(param);
					if (target)
						client->setOperator(channelName, true);
				}
				else
				{
					channel->removeOperator(param);
					Client*	target = findClientByNick(param);
					if (target)
						client->setOperator(channelName, false);
				}
			}
		}
		else if (mode == 'l')
		{
			if (adding && !param.empty())
			{
				int	usrLimit = atoi(param.c_str());
				if (usrLimit > 0)
					channel->setUserLimit(usrLimit);
			}
			else if (!adding)
				channel->setUserLimit(0);
		}
		std::string	modeMsg = ":" + client->getNickname() + " MODE " + channelName + " "
			+ (adding ? "+" : "-") + mode + (param.empty() ? "" : " " + param);
		const std::vector<std::string>&	members = channel->getMembers();
		for (size_t j = 0; j < members.size(); ++j)
		{
			Client* member = findClientByNick(members[j]);
			if (member)
				sendMessage(member->getFd(), modeMsg);
		}
	}
}

void    Server::handleNick()
{
	Client*	client = findClientByFd(_fds[_nbClients - 1].fd);
	if (!client)
		return ;

	std::vector<std::string>	args = _input.getArgs();
	if (args.empty())
	{
		sendMessage(client->getFd(), Errors::ERR_NONICKNAMEGIVEN(*client));
		return ;
	}

	std::string	newNick = args[0];
	// Validate nickname
	if (newNick.empty() || isdigit(newNick[0]) || newNick.find_first_of(" ,*?!@.#&") != std::string::npos)
	{
		sendMessage(client->getFd(), Errors::ERR_ERRONEUSNICKNAME(*client));
		return ;
	}

	if (findClientByNick(newNick))
	{
		sendMessage(client->getFd(), Errors::ERR_NICKNAMEINUSE(*client));
		return ;
	}

	std::string	oldNick = client->getNickname().empty() ? client->getClient() : client->getNickname();
	client->setNickname(newNick);
	Client	tempClient = *client;
	_clients.erase(oldNick);
	_clients[newNick] = tempClient;

	const std::vector<std::string>&	channels = client->getChannels();
	for (size_t i = 0; i < channels.size(); ++i)
	{
		Channel*	channel = findChannel(channels[i]);
		if (channel)
		{
			std::string	nickMsg = ":" + oldNick + " NICK " + newNick;
			const std::vector<std::string>&	members = channel->getMembers();
			for (size_t j = 0; j < members.size(); ++j)
			{
				Client*	member = findClientByNick(members[j]);
				if (member)
					sendMessage(member->getFd(), nickMsg);
			}
			channel->removeMember(oldNick);
			channel->addMember(newNick);
			if (client->isOperator(channels[i]))
			{
				channel->removeOperator(oldNick);
				channel->addOperator(newNick);
			}
		}
	}

	if (!oldNick.empty() && oldNick != client->getClient())
	{
		std::string	nickMsg = ":" + oldNick + " NICK " + newNick;
		sendMessage(client->getFd(), nickMsg);
	}
}

void    Server::handlePart()
{
	Client*	client = findClientByFd(_fds[_nbClients - 1].fd);
	if (!client || !client->isRegistered())
	{
		std::string	errMsg = ":" + _network_name + " 451 :You have not registered";
		sendMessage(_fds[_nbClients - 1].fd, errMsg);
		return ;
	}

	std::vector<std::string>	args = _input.getArgs();
	if (args.empty())
	{
		sendMessage(client->getFd(), Errors::ERR_NEEDMOREPARAMS(*client, _input));
		return ;
	}

	std::string	channelName = args[0];
	if (channelName[0] != '#')
		channelName = "#" + channelName;

	Channel*	channel = findChannel(channelName);
	if (!channel)
	{
		sendMessage(client->getFd(), Errors::ERR_NOSUCHCHANNEL(*client, *channel));
		return ;
	}
	if (!channel->isMember(client->getNickname()))
	{
		sendMessage(client->getFd(), Errors::ERR_NOTONCHANNEL(*client, *channel));
		return ;
	}

	channel->removeMember(client->getNickname());
	client->removeChannel(channelName);
	std::string	partMsg = ":" + client->getNickname() + " PART " + channelName;
	const std::vector<std::string>&	members = channel->getMembers();
	for (size_t i = 0; i < members.size(); ++i)
	{
		Client* member = findClientByNick(members[i]);
		if (member)
			sendMessage(member->getFd(), partMsg);
	}
	sendMessage(client->getFd(), partMsg);
}

void    Server::handlePass()
{
	Client*	client = findClientByFd(_fds[_nbClients - 1].fd);
	if (!client)
		return ;

	std::vector<std::string>	args = _input.getArgs();
	if (args.empty())
	{
		sendMessage(client->getFd(), Errors::ERR_NEEDMOREPARAMS(*client, _input));
		return ;
	}
	if (client->isRegistered())
	{
		sendMessage(client->getFd(), Errors::ERR_ALREADYREGISTERED(*client));
		return ;
	}
	if (args[0] != _password)
	{
		sendMessage(client->getFd(), Errors::ERR_PASSWDMISMATCH(*client));
		return ;
	}

	client->setPass(args[0]);
}

void    Server::handleTopic()
{
	Client*	client = findClientByFd(_fds[_nbClients - 1].fd);
	if (!client || !client->isRegistered())
	{
		std::string	errMsg = ":" + _network_name + " 451 :You have not registered";
		sendMessage(_fds[_nbClients - 1].fd, errMsg);
		return ;
	}

	std::vector<std::string>	args = _input.getArgs();
	if (args.empty())
	{
		Errors::ERR_NEEDMOREPARAMS(*client, _input);
		return ;
	}

	std::string	channelName = args[0];
	if (channelName[0] != '#')
		channelName = "#" + channelName;

	std::string	topic = args.size() < 1 ? args[1] : "";

	Channel*	channel = findChannel(channelName);
	if (!channel)
	{
		Errors::ERR_NOSUCHCHANNEL(*client, *channel);
		return ;
	}
	if (!channel->isMember(client->getNickname()))
	{
		Errors::ERR_NOTONCHANNEL(*client, *channel);
		return ;
	}
	if (topic.empty())
	{
		if (channel->getTopic().empty())
			Reply::RPL_NOTOPIC(*client, *channel);
		else
			Reply::RPL_TOPIC(*client, *channel);
		return ;
	}

	if (channel->getTopicRestricted() && !channel->isOperator(client->getNickname()))
	{
		Errors::ERR_CHANOPRIVSNEEDED(*client, *channel);
		return ;
	}

	channel->setTopic(topic);
	std::string	topicMsg = ":" +client->getNickname() + " TOPIC " + channelName + " :" + topic;
	const std::vector<std::string>&	members = channel->getMembers();
	for (size_t i = 0; i < members.size(); ++i)
	{
		Client*	member = findClientByNick(members[i]);
		if (member)
			sendMessage(member->getFd(), topicMsg);
	}
}

void    Server::handlePrivmsg()
{
	Client*	client = findClientByFd(_fds[_nbClients - 1].fd);

	if (!client || !client->isRegistered())
	{
		std::string	errMsg = ":" + _network_name + " 451 :You have not registered";
		sendMessage(_fds[_nbClients - 1].fd, errMsg);
		return ;
	}

	std::vector<std::string>	args = _input.getArgs();
	if (args.size() < 2)
	{
		sendMessage(client->getFd(), Errors::ERR_NEEDMOREPARAMS(*client, _input));
		return ;
	}

	std::string	target = args[0];
	std::string	message = args[1];
	if (message[0] == ':')
		message.erase(0, 1);

	if (target[0] == '#')
	{
		Channel*	channel = findChannel(target);
		if (!channel)
		{
			sendMessage(client->getFd(), Errors::ERR_NOSUCHCHANNEL(*client, *channel));
			return ;
		}
		if (!channel->isMember(client->getNickname()))
		{
			sendMessage(client->getFd(), Errors::ERR_NOTONCHANNEL(*client, *channel));
			return ;
		}
		
		std::string	privMsg = ":" + client->getNickname() + " PRIVMSG " + target + " :" + message;
		const std::vector<std::string>&	members = channel->getMembers();
		for (size_t i = 0; i < members.size(); ++i)
		{
			Client*	member = findClientByNick(members[i]);
			if (member)
				sendMessage(member->getFd(), privMsg);
		}
	}
	else
	{
		Client*	targetClient = findClientByNick(target);
		if (!targetClient)
		{
			std::string	errMsg = ":" + _network_name + " 401 " + client->getNickname() + " "
				+ target + " :No such nick/channel";
			sendMessage(client->getFd(), errMsg);
			return ;
		}
		std::string	privMsg = ":" + client->getNickname() + " PRIVMSG" + target + " :" + message;
		sendMessage(targetClient->getFd(), privMsg);
	}
}

void    Server::handleUser()
{
	Client*	client = findClientByFd(_fds[_nbClients - 1].fd);

	if (!client)
		return ;

	std::vector<std::string>	args = _input.getArgs();
	if (args.size() < 4)
	{
		sendMessage(client->getFd(), Errors::ERR_NEEDMOREPARAMS(*client, _input));
		return ;
	}
	if (client->isRegistered())
	{
		sendMessage(client->getFd(), Errors::ERR_ALREADYREGISTERED(*client));
		return ;
	}

	std::string	username = args[0];
	//Don't forget to ignore hostname and servername(args[1] and args[2]), use realname (args[3])
	client->setUsername(username);
	client->setRegistered(true);

	if (!client->getNickname().empty() && !client->getPass().empty())
		joinGreetings(_nbClients - 1);
}
