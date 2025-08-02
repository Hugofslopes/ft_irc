#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <vector>

class Channel
{
private:
	std::string	_name;
	std::string	_topic;
	bool		_key;
	std::string	_keyValue;
	bool		_invite;
	bool		_topicRestricted;
	int			_userLimit;

	std::vector<std::string>	_members;
	std::vector<std::string>	_operators;
	std::vector<std::string>	_invited;

public:
	Channel();
	Channel(std::string);
	Channel(const Channel&);
	Channel& operator=(const Channel&);
	~Channel();


	//Getters
	std::string	getName() const;
	std::string	getTopic() const;
	bool		getKey() const;
	std::string	getKeyValue() const;
	bool		getInvite() const;
	bool		getTopicRestricted() const;
	int			getUserLimit() const;

	const std::vector<std::string>&	getMembers() const;
	const std::vector<std::string>&	getOperators() const;
	const std::vector<std::string>&	getInvited() const;

	//Member management
	bool	addMember(const std::string& nick);
	bool	removeMember(const std::string& nick);
	bool	isMember(const std::string& nick) const;

	bool	addOperator(const std::string& nick);
	bool	removeOperator(const std::string& nick);
	bool	isOperator(const std::string& nick) const;

	bool	addInvited(const std::string& nick);
	bool	isInvited(const std::string& nick) const;

	//Mode management
	void	setKey(const std::string& key);
	void	removeKey();
	void	setInvite(bool status);
	void	setTopicRestricted(bool status);
	void	setUserLimit(int limit);
	void	setTopic(const std::string& topic);
};

#endif
