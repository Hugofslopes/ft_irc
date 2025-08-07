#include "../includes/Client.hpp"

Client::Client() : fd(-1), registered(false) {}

Client::Client(int fd) : fd(fd), registered(false) {}

Client::Client(const Client &other) {
    client = other.client;
    username = other.username;
    nick = other.nick;
    pass = other.pass;
    fd = other.fd;
    registered = other.registered;
    operatorStatus = other.operatorStatus;
    channels = other.channels;
}

Client &Client::operator=(const Client &other) {
    if (this != &other) {
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

Client::~Client() {}

std::string Client::getClient() const { return (client); }

std::string Client::getUsername() const { return (username); }

std::string Client::getNickname() const { return (nick); }

int Client::getFd() const { return (fd); }

bool Client::isRegistered() const { return (registered); }

bool Client::isOperator(const std::string &channel) const {
    std::map<std::string, bool>::const_iterator it =
        operatorStatus.find(channel);
    return (it != operatorStatus.end() && it->second);
}

const std::vector<std::string> &Client::getChannels() const {
    return (channels);
}

void Client::setClient(const std::string &client) { this->client = client; }

void Client::setUsername(const std::string &username) {
    this->username = username;
}

void Client::setNickname(const std::string &nick) { this->nick = nick; }

void Client::setRegistered(bool status) { registered = status; }

void Client::setOperator(const std::string &channel, bool status) {
    operatorStatus[channel] = status;
}

void Client::addChannel(const std::string &channel) {
    if (std::find(channels.begin(), channels.end(), channel) == channels.end())
        channels.push_back(channel);
}

void Client::removeChannel(const std::string &channel) {
    std::vector<std::string>::iterator it =
        std::find(channels.begin(), channels.end(), channel);

    if (it != channels.end()) {
        channels.erase(it);
        operatorStatus.erase(channel);
    }
}

void	Client::appendPartialMessage(const std::string &message) {
	_partialMessage += message;
	if (_input.isComplete(_partialMessage)) {
		_input.parse(_partialMessage);
		_partialMessage.clear();
	}
}

//Verify if it is not Partial
bool	Client::processMessage(const std::string &message) {
        _partialMessage += message;

        if (!getInput().isComplete(_partialMessage))
            return false;
        _input.parse(_partialMessage);
        _partialMessage.clear();
        return true;
}

//IN PROGRESS
//ONLY IF CLIENT IS NOT REGISTERED
bool	Client::processInitialCommands() {

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
			handlePass();
		else if (_input.getCommand() == "NICK")
			handleNick();
		else if (_input.getCommand() == "USER")
			handleUser();

		registered = true;
		return true;
	}
	return false;
}

void Client::clearInput() {return _input.clear();}
