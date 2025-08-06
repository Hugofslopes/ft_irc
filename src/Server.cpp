#include "../includes/Server.hpp"
#include "../includes/Input.hpp"
#include "../includes/Reply.hpp"
#include <cctype>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <vector>

Server* Server::instance = NULL;
//<<<<<<<<<<<<<<<<<<<<<<CONSTRUCTORS>>>>>>>>>>>>>>>>>>>>>>>>
Server::Server(std::string port, std::string password){
    _network_name = "ft_irc";
    _version = "1.0";
    parsePort(port);
    _password = password;
    _nbClients = 0; 
    _socketfd = 0;
    memset(_fds, 0, sizeof(_fds));
    Server::instance = this;
}

Server::Server(){}

Server::~Server(){
    closeExit();
}

Server::Server(const Server& other){
    _network_name = other._network_name;
    _version = other._version;
    _StartTime = other._StartTime;
    _clients = other._clients;
    _channels = other._channels;
    _nbClients = other._nbClients;
    _socketfd = other._socketfd;
}

Server& Server::operator=(const Server& other){
    if (this != &other)
    {
        _network_name = other._network_name;
        _version = other._version;
        _StartTime = other._StartTime;
        _clients = other._clients;
        _channels = other._channels;    
        _nbClients = other._nbClients;
        _socketfd = other._socketfd;
    }
    return *this;
}

//<<<<<<<<<<<<<<<<<<<<<<GETERS>>>>>>>>>>>>>>>>>>>>>>>>
std::string Server::getName() const{
    return _network_name;
}

std::string Server::getVersion() const{
    return _version;
}

std::string Server::getStTime() const{
    return _StartTime;
}

//<<<<<<<<<<<<<<<<<<<<<<SERVER>>>>>>>>>>>>>>>>>>>>>>>>
void Server::start()
{
    _socketfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (_socketfd == -1)
        throw std::runtime_error("Failed to create socket");

    int off = 0;
    if (setsockopt(_socketfd, IPPROTO_IPV6, IPV6_V6ONLY, &off, sizeof(off)) < 0)
        throw std::runtime_error("Failed to set socket options");

    sockaddr_in6 serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin6_family = AF_INET6;
    serverAddr.sin6_port = htons(_port);
    serverAddr.sin6_addr = in6addr_any;

    if (bind(_socketfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
        throw std::runtime_error("Bind failed");

    if (listen(_socketfd, 5) < 0)
        throw std::runtime_error("Listen failed");
    
    _fds[0].fd = _socketfd;
    _fds[0].events = POLLIN;
    _nbClients = 1;
}

void Server::run(){
    struct sigaction sa;
    sa.sa_handler = handleSigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    setDateTime();
    signalIgnore();
    while (1)
    {
        if (poll(_fds, _nbClients, -1) == -1)
            std::cerr << "Poll error" << std::endl, exit(1); 
        for (int i = 0; i <= _nbClients; i++)
		{
			if (_fds[i].revents & POLLIN)
			{
                if (_fds[i].fd == _socketfd)
                {
                    std::cout <<"NEW"<< std::endl;
					newClient();
                }
				else
                {
					clientRequest(i);
                }
            }
        }
    }
}

void    Server::newClient(){
    sockaddr_storage clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    int clientFd = accept(_socketfd, (sockaddr*)&clientAddr, &clientLen);
    if (clientFd == -1) {
        std::cerr << "Failed to accept new client\n";
        return;
    }

    fcntl(clientFd, F_SETFL, O_NONBLOCK);

    _fds[_nbClients].fd = clientFd;
    _fds[_nbClients].events = POLLIN;
    _nbClients++;
    Client client(clientFd);
    std::stringstream   ss;
    ss << "temp_" << clientFd;
    client.setClient(ss.str());
    _clients[ss.str()] = client;
    //Client client(_nbClients);
    //_clients.push_back(client);
}

void    Server::clientRequest(int index){
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    int bytesRead = recv(_fds[index].fd, buffer, sizeof(buffer) - 1, 0);
    std::cout << "BYTESREAD- " << bytesRead << std::endl;
    if (bytesRead <= 0)
    {
        Client* client = findClientByFd(_fds[index].fd);
        if (client)
        {
            std::string nick = client->getNickname();
            const std::vector<std::string>& channels = client->getChannels();
            for (size_t i = 0; i < channels.size(); ++i)
            {
                Channel*    channel = findChannel(channels[i]);
                if (channel)
                {
                    channel->removeMember(nick);
                    std::string partMsg = ":" + nick + " PART " + channels[i];
                    const std::vector<std::string>& members = channel->getMembers();
                    for (size_t j = 0; j < members.size(); ++j)
                    {
                        Client* member = findClientByNick(members[j]);
                        if (member)
                            sendMessage(member->getFd(), partMsg);
                    }
                }
            }
            _clients.erase(nick.empty() ? client->getClient() : nick);
        }
        close(_fds[index].fd);
        _fds[index].fd = -1;
        std::cout << "Client disconnected\n";
        return;
    }

    std::string message(buffer, bytesRead);
    std::cout << "MESSAGE- " << message << std::endl;
    _input = Input(message);
    Client* client = findClientByFd(_fds[index].fd);

    if (client && !client->isRegistered())
        process_login();
    else
        executeCommand();
}

void Server::executeCommand(){
    std::string cmd = _input.getCommand();
    const std::string commands[] = {
        "INVITE", "JOIN", "KICK", "MODE", "NICK", "PART", "PASS", 
        "PRIVMSG", "TOPIC", "USER"
    };

    void (Server::*handlers[])() = {
    &Server::handleInvite,
    &Server::handleJoin,
    &Server::handleKick,
    &Server::handleMode,
    &Server::handleNick,
    &Server::handlePart,
    &Server::handlePass,
    &Server::handlePrivmsg,
    &Server::handleTopic,
    &Server::handleUser,
    };

    const int commandCount = sizeof(commands) / sizeof(commands[0]);
    std::cout << "Raw cmd: [" << cmd << "]" << std::endl;
    for (int i = 0; i < commandCount; ++i){
        if (commands[i] == cmd) {
            (this->*handlers[i])();
            return;
        }
    }

    std::cerr << "Command not found" << std::endl;

    Client* client = findClientByFd(_fds[_nbClients - 1].fd);

    if (client)
    {
        std::string errMsg = ":" + _network_name + " 421 " + client->getNickname() + cmd + " :Unknown command";
        sendMessage(client->getFd(), errMsg);
    }
}

void    Server::process_login()
{
    //TODO: Luigi: trigger joinGreetings when client is registered: PASS, NICK, USER
}

//<<<<<<<<<<<<<<<<<<<<<<UTILS>>>>>>>>>>>>>>>>>>>>>>>>
void Server::joinGreetings(int index)
{
    std::string reply = Reply::RPL_WELCOME(_clients[index - 1], *this);
    send(_fds[index].fd, reply.c_str(), reply.length(), 0);
    reply =  Reply::RPL_YOURHOST(_clients[index - 1], *this);
    send(_fds[index].fd, reply.c_str(), reply.length(), 0);
    reply =  Reply::RPL_CREATED(_clients[index - 1], *this);
    send(_fds[index].fd, reply.c_str(), reply.length(), 0);
}
 
void Server::setDateTime(){
    time_t now = time(0);
    struct tm* local = localtime(&now);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", local);

    std::string timeStr(buffer);
    _StartTime = timeStr;
}

void signalIgnore(){
    signal(SIGQUIT, SIG_IGN);
}

void    Server::closeExit(){
    _clients.clear();
    std::vector<Client>().swap(_clients);

    _channels.clear();
    std::vector<Channel>().swap(_channels);

    if (_socketfd != -1)
    close(_socketfd);

    for (int i = 0; i <= _nbClients; ++i)
    if (_fds[i].fd != -1)
        close(_fds[i].fd);
}

void handleSigint(int sig){
    (void)sig;
    if (Server::instance)
        Server::instance->closeExit();  
    exit(0);
}

void Server::parsePort(std::string port){
    for (size_t i = 0; i < port.size(); ++i) 
        if (!std::isdigit(port[i]))
            std::cerr << "Invalid port\n", exit(1);

    std::stringstream ss(port);
    int number;
    ss >> number;

    if (number < 1024 || number > 65535)
        std::cerr << "Invalid port\n", exit(1);
    _port = number;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<HELPER METHODS>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

Client* Server::findClientByFd(int fd)
{
    for (std::map<std::string, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second.getFd() == fd)
            return (&(it->second));
    }
    return (NULL);
}

Client* Server::findClientByNick(const std::string& nick)
{
    std::map<std::string, Client>::iterator it = _clients.find(nick);
    return (it != _clients.end() ? &(it->second) : NULL);
}

Channel*    Server::findChannel(const std::string& name)
{
    std::map<std::string, Channel>::iterator    it = _channels.find(name);
    return (it != _channels.end() ? &(it->second) : NULL);
}

void    Server::sendMessage(int fd, const std::string& message)
{
    std::string msg = message + "\r\n";
    send(fd, msg.c_str(), msg.length(), 0);
}

//<<<<<<<<<<<<<<<<<<<<<<EXECUTE COMMANDS/HANDLERS>>>>>>>>>>>>>>>>>>>>>>>>

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
		_channels[channelName] = Channel[channelName];
		channel = findChannel(channelName);
		channel->addMember(client->getNickname());
		channel->addOperator(client->getNickname()); //the creator is the default operator
	}
	else
	{
		if (channel->getUserLimit() > 0 && static_cast<int>(channel->getMembers().size() >=channel->getUserLimit()))
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
