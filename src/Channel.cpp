#include "../includes/Channel.hpp"

Channel::Channel() : _key(false), _invite(false), _topicRestricted(false), 
_userLimit(0), _topic("default"){}

Channel::Channel(const std::string& name) : _name(name), _key(false), _invite(false),
_topicRestricted(false), _userLimit(0), _topic("default"){}

Channel::Channel(const Channel &other)
{
	_name = other._name;
	_topic = other._topic;
	_key = other._key;
	_keyValue = other._keyValue;
	_invite = other._invite;
	_topicRestricted = other._topicRestricted;
	_userLimit = other._userLimit;
	_members = other._members;
	_operators = other._operators;
	_invited = other._invited;
}

Channel	&Channel::operator=(const Channel &other)
{
	if (this != &other)
	{
		_name = other._name;
		_topic = other._topic;
		_key = other._key;
		_keyValue = other._keyValue;
		_invite = other._invite;
		_topicRestricted = other._topicRestricted;
		_userLimit = other._userLimit;
		_members = other._members;
		_operators = other._operators;
		_invited = other._invited;
	}
	return (*this);
}

Channel::~Channel()
{
}

std::string	Channel::getName() const
{
	return (_name);
}

std::string	Channel::getTopic() const
{
	return (_topic);
}

bool	Channel::getKey() const
{
	return (_key);
}

std::string	Channel::getKeyValue() const
{
	return (_keyValue);
}

bool	Channel::getInvite() const
{
	return (_invite);
}

bool	Channel::getTopicRestricted() const
{
	return (_topicRestricted);
}

int	Channel::getUserLimit() const
{
	return (_userLimit);
}

const std::vector<std::string>&	Channel::getMembers() const
{
	return (_members);
}

const std::vector<std::string>&	Channel::getOperators() const
{
	return (_operators);
}

const std::vector<std::string>&	Channel::getInvited() const
{
	return (_invited);
}

bool	Channel::addMember(const std::string& nick)
{
	if (!isMember(nick))
	{
		_members.push_back(nick);
		return (true);
	}
	return (false);
}

bool	Channel::removeMember(const std::string& nick)
{
	std::vector<std::string>::iterator	it = std::find(_members.begin(), _members.end(), nick);

	if (it != _members.end())
	{
		_members.erase(it);
		removeOperator(nick);
		return (true);
	}
	return (false);
}

bool	Channel::isMember(const std::string& nick) const
{
	return (std::find(_members.begin(), _members.end(), nick) != _members.end());
}

bool	Channel::addOperator(const std::string& nick)
{
	if (isMember(nick) && !isOperator(nick))
	{
		_operators.push_back(nick);
		return (true);
	}
	return (false);
}

bool	Channel::removeOperator(const std::string& nick)
{
	std::vector<std::string>::iterator	it = std::find(_operators.begin(), _operators.end(), nick);

	if (it != _operators.end())
	{
		_operators.erase(it);
		return (true);
	}
	return (false);
}

bool	Channel::isOperator(const std::string& nick) const
{
	return (std::find(_operators.begin(), _operators.end(), nick) != _operators.end());
}

bool	Channel::addInvited(const std::string& nick)
{
	if (!isInvited(nick))
	{
		_invited.push_back(nick);
		return (true);
	}
	return (false);
}

bool	Channel::isInvited(const std::string& nick) const
{
	return (std::find(_invited.begin(), _invited.end(), nick) != _invited.end());
}

void	Channel::setKey(const std::string& key)
{
	_key = !key.empty();
	_keyValue = key;
}

void	Channel::removeKey()
{
	_key = false;
	_keyValue.clear();
}

void	Channel::setInvite(bool status)
{
	_invite = status;
}

void	Channel::setTopicRestricted(bool status)
{
	_topicRestricted = status;
}

void	Channel::setUserLimit(int limit)
{
	_userLimit = limit;
}

void	Channel::setTopic(const std::string& topic)
{
	_topic = topic;
}

 size_t Channel::getMemberCount() const {
        return _members.size();
 }