#include "../includes/Server.hpp"
#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include "../includes/Reply.hpp"
#include "../includes/Errors.hpp"

void	Server::handleInvite(Client *client, std::vector<std::string> args)
{
	if (args.size() < 3)
	{
		sendMessage(client->getFd(), Errors::ERR_NEEDMOREPARAMS(*client,  client->_input));
		return ;
	}

	std::string	targetNick = args[1];
	std::string	channelName = args[2];

	Channel*	channel = findChannel(channelName);
	if (!channel)
	{
		sendMessage(client->getFd(), Errors::ERR_NOSUCHCHANNEL(*client, channelName));
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
	sendMessage(client->getFd(), Reply::RPL_INVITING(*client, *channel, targetNick, *this));
	std::string	inviteMsg = ":" + client->getNickname() + " INVITE " + targetNick + " :" + channelName;
	Client * client2 = findClientByNick(targetNick);
	sendMessage(client2->getFd(), inviteMsg);
}

void	Server::handleJoin(Client *client, std::vector<std::string> args)
{
	if (args.empty() || args[1].empty())
	{
		sendMessage(client->getFd(), Errors::ERR_NEEDMOREPARAMS(*client,  client->_input));
		return ;
	}

	std::string	channelName = args[1];
	if (channelName[0] != '#'){
		sendMessage(client->getFd(),Errors::ERR_NOSUCHCHANNEL(*client, channelName));
		return;
	}

	std::string	key = args.size() > 2 ? args[2] : "";

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
		if (channel->getInvite() && !channel->isInvited(client->getNickname()))
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

	const std::vector<std::string>&	members = channel->getMembers();
	for (size_t i = 0; i < members.size(); ++i)
	{
		Client*	member = findClientByNick(members[i]);
		if (member && member != client)
			sendMessage(member->getFd(), Reply::RPL_JOINEDCHA(*client,*channel));
	}

	sendMessage(client->getFd(),Reply::RPL_JOINEDCHA(*client,*channel));
	sendMessage(client->getFd(),Reply::RPL_JOINEDCHATOPIC(*this,*client,*channel));
	sendMessage(client->getFd(),Reply::RPL_NAMREPLY(*client,*channel));
	sendMessage(client->getFd(),Reply::RPL_ENDOFNAMES(*client,*channel));
}

void	Server::handleKick(Client *client, std::vector<std::string> args)
{
	if (args.size() < 3)
	{
		sendMessage(client->getFd(), Errors::ERR_NEEDMOREPARAMS(*client,  client->_input));
		return ;
	}

	std::string	channelName = args[1];
	std::string	targetNick = args[2];
	std::string	reason = args.size() > 3 ? args[3] : "No reason";

	Channel*	channel = findChannel(channelName);
	if (!channel)
	{
		sendMessage(client->getFd(), Errors::ERR_NOSUCHCHANNEL(*client, channelName));
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

void	Server::handleMode(Client *client, std::vector<std::string> args)
{
	if (args.empty() || args.size() < 2)
	{
		sendMessage(client->getFd(), Errors::ERR_NEEDMOREPARAMS(*client,  client->_input));
		return ;
	}

	std::string	channelName = args[1];
	std::string	modeStr;
	if (args.size() > 2)
		modeStr = args.size() > 1 ? args[2] : "";

	Channel*	channel = findChannel(channelName);
	if (!channel)
	{
		sendMessage(client->getFd(), Errors::ERR_NOSUCHCHANNEL(*client, channelName));
		return ;
	}
	if (channel && args.size() == 2)
	{
		sendMessage(client->getFd(), Reply::RPL_MODE(*this, *client, *channel));
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
		std::string	modeMsg = "324 " + client->getNickname() + ' ' + channel->getName() + ' ' + modes;
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
		std::string	param;
		if (args.size() > paramIndx + 1 && (mode == 'o' || mode == 'k' || mode == 'l')) 
			param = args[++paramIndx];
		else
			param = "";
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
				if (channel->isMember(param) == false)
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

int   Server::handleNick(Client *client, std::vector<std::string> args)
{	
	if (args.empty())
	{
		sendMessage(client->getFd(), Errors::ERR_NONICKNAMEGIVEN(*client));
		return 1;
	}
	std::string	newNick = args[1];
	// Validate nickname
	if (newNick.empty() || isdigit(newNick[0]) || newNick.find_first_of(" ,*?!@.#&") != std::string::npos)
	{
		sendMessage(client->getFd(), Errors::ERR_ERRONEUSNICKNAME(*client));
		return 1;
	}
	if (findClientByNick(newNick))
	{
		sendMessage(client->getFd(), Errors::ERR_NICKNAMEINUSE(*client));
		return 1;
	}
	
	std::string oldNick = client->getNickname().empty() ? client->getClient() : client->getNickname();
	_clients.erase(oldNick);
	client->setNickname(newNick);
	_clients[newNick] = client;

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
	std::string identity;
	if (client->getUsername().empty())
		identity = oldNick + "!" + client->getNickname()+ "@localhost";
	else
		identity = oldNick;
	std::string nickMsg = ":" + identity + " NICK :" + newNick;
	sendMessage(client->getFd(), nickMsg);
	return 0;
}

void    Server::handlePart(Client *client, std::vector<std::string> args)
{
	if (args.empty())
	{
		sendMessage(client->getFd(), Errors::ERR_NEEDMOREPARAMS(*client,  client->_input));
		return ;
	}

	std::string	channelName = args[1];

	Channel*	channel = findChannel(channelName);
	if (!channel)
	{
		sendMessage(client->getFd(), Errors::ERR_NOSUCHCHANNEL(*client, channelName));
		return ;
	}
	if (!channel->isMember(client->getNickname()))
	{
		sendMessage(client->getFd(), Errors::ERR_NOTONCHANNEL(*client, *channel));
		return ;
	}

	channel->removeMember(client->getNickname());
	client->removeChannel(channelName);
	const std::vector<std::string>&	members = channel->getMembers();
	for (size_t i = 0; i < members.size(); ++i)
	{
		Client* member = findClientByNick(members[i]);
		if (member)
			sendMessage(member->getFd(), Reply::RPL_PART(*client, *channel));
	}
	sendMessage(client->getFd(), Reply::RPL_PART(*client, *channel));
}

int    Server::handlePass(Client *client, std::vector<std::string> args)
{
	if (args.empty())
	{
		sendMessage(client->getFd(), Errors::ERR_NEEDMOREPARAMS(*client,  client->_input));
		return 1;
	}
	if (client->isRegistered())
	{
		sendMessage(client->getFd(), Errors::ERR_ALREADYREGISTERED(*client));
		return 1;
	}
	if (args[1] != _password)
	{
		sendMessage(client->getFd(), Errors::ERR_PASSWDMISMATCH(*client));
		return 1;
	}

	client->setPass(args[1]);
	return 0;
}

void    Server::handleTopic(Client *client, std::vector<std::string> args)
{
	if (args.empty())
	{
		sendMessage(client->getFd(), Errors::ERR_NEEDMOREPARAMS(*client,  client->_input));
		return ;
	}

	std::string	channelName = args[1];

	std::string	topic = args.size() < 3 ?  "" : args[2] ;

	Channel*	channel = findChannel(channelName);
	if (!channel)
	{
		sendMessage(client->getFd(), Errors::ERR_NOSUCHCHANNEL(*client, channelName));
		return ;
	}
	if (!channel->isMember(client->getNickname()))
	{
		sendMessage(client->getFd(), Errors::ERR_NOTONCHANNEL(*client, *channel));
		return ;
	}
	if (topic.empty())
	{
		if (channel->getTopic().empty())
			sendMessage(client->getFd(), Reply::RPL_NOTOPIC(*client, *channel));
		else
			sendMessage(client->getFd(), Reply::RPL_TOPIC(*client, *channel, topic));
		return ;
	}

	if (channel->getTopicRestricted() && !channel->isOperator(client->getNickname()))
	{
		sendMessage(client->getFd(), Errors::ERR_CHANOPRIVSNEEDED(*client, *channel));
		return ;
	}

	channel->setTopic(topic);
	std::string	topicMsg = Reply::RPL_TOPIC(*client, *channel, topic);
	const std::vector<std::string>&	members = channel->getMembers();
	for (size_t i = 0; i < members.size(); ++i)
	{
		Client*	member = findClientByNick(members[i]);
		if (member)
			sendMessage(member->getFd(), topicMsg);
	}
}

void    Server::handlePrivmsg(Client *client, std::vector<std::string> args)
{
	if (args.size() < 2)
	{
		sendMessage(client->getFd(), Errors::ERR_NEEDMOREPARAMS(*client,  client->_input));
		return ;
	}

	std::string	target = args[1];
	std::string	message;
	std::vector<std::string>::iterator it = args.begin();
	if (args.size() > 1)
		it += 2;
	for (int i = 0; it != args.end(); it++, i++){
		if (i != 0)
			message += ' ';
		message += *it;
	}
	if (message[0] == ':')
		message.erase(0, 1);

	if (target[0] == '#')
	{
		Channel*	channel = findChannel(target);
		if (!channel)
		{
			sendMessage(client->getFd(), Errors::ERR_NOSUCHCHANNEL(*client, target));
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
			if (member && member->getFd() != client->getFd())
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
		std::string	privMsg = ":" + client->getNickname() + " PRIVMSG " + target + " :" + message;
		sendMessage(targetClient->getFd(), privMsg);
	}
}

int    Server::handleUser(Client *client, std::vector<std::string> args)
{
	if (args.size() < 4)
	{
		sendMessage(client->getFd(), Errors::ERR_NEEDMOREPARAMS(*client, client->_input));
		return 1;
	}
	if (client->isRegistered())
	{
		sendMessage(client->getFd(), Errors::ERR_ALREADYREGISTERED(*client));
		return 1;
	}

	std::string	username = args[1];
	//Don't forget to ignore hostname and servername(args[1] and args[2]), use realname (args[3])
	client->setUsername(username);
	return 0;
}
void    Server::handleWho(Client *client, std::vector<std::string> args)
{
	if (args.size() < 2)
	{
		sendMessage(client->getFd(), Errors::ERR_NEEDMOREPARAMS(*client,  client->_input));
		return ;
	}

	Channel*	channel = findChannel(args[1]);
	if (!channel)
	{
		sendMessage(client->getFd(), Errors::ERR_NOSUCHCHANNEL(*client, args[1]));
		return ;
	}

	sendMessage(client->getFd(), Reply::RPL_WHO_REPLY(*this,*channel,*client));
	sendMessage(client->getFd(), Reply::RPL_WHO_END_REPLY(*this,*channel,*client));
}

void	Server::handleQuit(Client *client, std::vector<std::string> args){
	std::string msg;
	std::vector<std::string>::iterator it = args.begin();
	if (args.size() > 1)
		it += 2;
	for (int i = 0; it != args.end(); it++, i++){
		if (i > 0)
			msg += ' ';
		msg += *it;
	}

	std::vector<std::string> channels = client->getChannels();
	if (!channels.empty())
	{
		std::vector<std::string> msgSent;
		std::vector<std::string>::iterator itch = channels.begin();

		for (; itch != channels.end(); itch++){
			Channel*	channel = findChannel(*itch);
			std::vector<std::string>  channelMemb = channel->getMembers();
			std::vector<std::string>::iterator  itc = channelMemb.begin();

			for (; itc != channelMemb.end(); itc++){
				if (std::find(msgSent.begin(), msgSent.end(), *itc) == msgSent.end()){
					 msgSent.push_back(*itc);
					Client* member = findClientByNick(*itc);
					sendMessage(member->getFd(), Reply::RPL_QUIT(*client,msg));
				}
			}
		}
	}

	int index = findFdIndex(client->getFd());
	removeClient(client, index);
}