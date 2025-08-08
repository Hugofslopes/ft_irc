#include "../includes/Client.hpp"
#include <string>

Client::Client() : fd(-1), registered(false){}

Client::Client(int fd) : fd(fd), registered(false){}

Client::Client(const Client &other)
{
	client = other.client;
	username = other.username;
	nick = other.nick;
	pass = other.pass;
	fd = other.fd;
	registered = other.registered;
	operatorStatus = other.operatorStatus;
	channels = other.channels;
}

Client	&Client::operator=(const Client &other)
{
	if (this != &other)
	{
		client = other.client;
		username = other.username;
		nick = other.nick;
		pass = other.pass;
		fd = other.fd;
		registered = other.registered;
		operatorStatus = other.operatorStatus;
		channels = other.channels;
	}
	return (*this);
}

Client::~Client()
{
}

std::string	Client::getClient() const
{
	return (client);
}

std::string	Client::getUsername() const
{
	return (username);
}

std::string	Client::getNickname() const
{
	return (nick);
}

std::string	Client::getPass() const
{
	return (pass);
}

int	Client::getFd() const
{
	return (fd);
}

bool	Client::isRegistered() const
{
	return (registered);
}

bool	Client::isOperator(const std::string &channel) const
{
	std::map<std::string, bool>::const_iterator it =
		operatorStatus.find(channel);
	return (it != operatorStatus.end() && it->second);
}

const std::vector<std::string>	&Client::getChannels() const
{
	return (channels);
}

void	Client::setClient(const std::string &client)
{
	this->client = client;
}

void	Client::setUsername(const std::string &username)
{
	this->username = username;
}

void	Client::setNickname(const std::string &nick)
{
	this->nick = nick;
}

void	Client::setPass(const std::string& pass)
{
	this->pass = pass;
}

void	Client::setRegistered(bool status)
{
	registered = status;
}

void	Client::setOperator(const std::string &channel, bool status)
{
	operatorStatus[channel] = status;
}

void	Client::addChannel(const std::string &channel)
{
	if (std::find(channels.begin(), channels.end(), channel) == channels.end())
		channels.push_back(channel);
}

void	Client::removeChannel(const std::string &channel)
{
	std::vector<std::string>::iterator it =
		std::find(channels.begin(), channels.end(), channel);

	if (it != channels.end())
	{
		channels.erase(it);
		operatorStatus.erase(channel);
	}
}

//IN PROGRESS
//ONLY IF CLIENT IS NOT REGISTERED
bool	Client::processInitialCommands(Server& serv) {

	size_t pos;

	while ((pos = _buffer.find("\r\n")) != std::string::npos)
	{
		std::string cmd = _buffer.substr(0, pos);
		_buffer.erase(0, pos + 2);

		_input.parse(cmd);
		if (!_input.isValid())
			return false;

		//does this makes sense...? Acho que deveria inicializar no parser e depois executa (?)
		if (_input.getCommand() == "PASS")
			serv.handlePass();
		else if (_input.getCommand() == "NICK")
			serv.handleNick();
		else if (_input.getCommand() == "USER")
			serv.handleUser();

		registered = true;
		return true;
	}
	return false;
}

void Client::clearInput() {
	return _input.clear();
}

const Input& Client::getInput() const {
	return _input; 
}

Input&	Client::getInput() { 
	return _input; 
}

void	Client::appendPartialMessage(const std::string &message){
	(void)message;
}

bool	Client::processMessage(const std::string &message){
	(void)message;
	return true;
}