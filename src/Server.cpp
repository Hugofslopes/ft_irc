#include "../includes/Server.hpp"

Server*	Server::instance = NULL;
//<<<<<<<<<<<<<<<<<<<<<<CONSTRUCTORS>>>>>>>>>>>>>>>>>>>>>>>>
Server::Server(std::string port, std::string password)
{
	_network_name = "ft_irc";
	_version = "1.0";
	parsePort(port);
	_password = password;
	_nbClients = 0;
	_socketfd = 0;
	memset(_fds, 0, sizeof(_fds));
	Server::instance = this;
}

Server::Server()
{
}

Server::~Server()
{
	closeExit();
}

Server::Server(const Server& other)
{
	_network_name = other._network_name;
	_version = other._version;
	_StartTime = other._StartTime;
	_clients = other._clients;
	_channels = other._channels;
	_nbClients = other._nbClients;
	_socketfd = other._socketfd;
}

Server&	Server::operator=(const Server& other)
{
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
	return (*this);
}

//<<<<<<<<<<<<<<<<<<<<<<GETERS>>>>>>>>>>>>>>>>>>>>>>>>
std::string	Server::getName() const
{
	return (_network_name);
}

std::string	Server::getVersion() const
{
	return (_version);
}

std::string	Server::getStTime() const
{
	return (_StartTime);
}

//<<<<<<<<<<<<<<<<<<<<<<SERVER>>>>>>>>>>>>>>>>>>>>>>>>
void	Server::start()
{
	_socketfd = socket(AF_INET6, SOCK_STREAM, 0);
	if (_socketfd == -1)
		throw std::runtime_error("Failed to create socket");
	
    int	off = 0;
	if (setsockopt(_socketfd, IPPROTO_IPV6, IPV6_V6ONLY, &off, sizeof(off)) < 0)
		throw std::runtime_error("Failed to set socket options");

	sockaddr_in6	serverAddr;
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

void	Server::run()
{
	struct sigaction	sa;
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
					std::cout <<"New client"<< std::endl;
					newClient();
				}
				else
					clientRequest(i);
            }
        }
    }
}

void	Server::newClient()
{
	sockaddr_storage	clientAddr;
	socklen_t			clientLen = sizeof(clientAddr);

	int	clientFd = accept(_socketfd, (sockaddr*)&clientAddr, &clientLen);
	if (clientFd == -1)
	{
		std::cerr << "Failed to accept new client\n";
		return;
	}

	fcntl(clientFd, F_SETFL, O_NONBLOCK);

	_fds[_nbClients].fd = clientFd;
	_fds[_nbClients].events = POLLIN;
	_nbClients++;
	Client* client = new Client(clientFd);
	std::stringstream	ss;
	ss << "temp_" << clientFd;
	client->setClient(ss.str());
	_clients[ss.str()] = client; 
}

void Server::removeClient(Client *client, int index)
{
	std::string	nick = client->getNickname();
	const std::vector<std::string>&	channels = client->getChannels();
	for (size_t i = 0; i < channels.size(); ++i)
	{
		Channel*	channel = findChannel(channels[i]);
		if (channel)
		{
			channel->removeMember(nick);
			std::string	partMsg = ":" + nick + " PART " + channels[i];
			const std::vector<std::string>&	members = channel->getMembers();
			for (size_t j = 0; j < members.size(); ++j)
			{
				Client*	member = findClientByNick(members[j]);
				if (member)
					sendMessage(member->getFd(), partMsg);
			}
		}
	}
	for (std::map<std::string, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->second == client) {
			_clients.erase(it);
			break;
		}
	}
	delete client;
	close(_fds[index].fd);
	_fds[index].fd = -1;
	std::cout << "Client disconnected\n";
}

void	Server::clientRequest(int index)
{
	char	buffer[1024];
	memset(buffer, 0, sizeof(buffer));

	int	bytesRead = recv(_fds[index].fd, buffer, sizeof(buffer) - 1, 0);
	std::cout << "BYTESREAD- " << bytesRead << " FDS " << _fds[index].fd << 
	" index " << index << std::endl;
	std::cout << "BUFFER[" <<buffer <<']'<< std::endl;
	Client*	client = findClientByFd(_fds[index].fd);
	if (bytesRead <= 0)
	{	
		if (_fds[index].fd != -1)
			removeClient(client, index);
		return ;
	}
	std::string message(buffer, bytesRead);
	std::vector<std::string> args;

	if (!client->isRegistered() && _fds[index].fd != -1)
		processRegister(client, message, index);
	else{
		client->_input.processInput(message);
		if (client->_input.getRaw().find("\n") != std::string::npos){
			while (!client->_input.getRaw().empty()){
				args = client->_input.process_args();
				executeCommand(client, args);
				if (_fds[index].fd == -1)
					return;
			}
		}
		else
			return;
	}
}

void	Server::executeCommand(Client *client, std::vector<std::string> args)
{
	std::string cmd = args[0];
	const std::string	commands[] = {
		"INVITE", "JOIN", "KICK", "MODE", "PART",
		"PRIVMSG", "TOPIC", "WHO", "QUIT"
	};

	void	(Server::*handlers[])(Client *client, std::vector<std::string> args) = {
	&Server::handleInvite,
	&Server::handleJoin,
	&Server::handleKick,
	&Server::handleMode,
	&Server::handlePart,
	&Server::handlePrivmsg,
	&Server::handleTopic,
	&Server::handleWho,
	&Server::handleQuit
	};

	const int	commandCount = sizeof(commands) / sizeof(commands[0]);
	for (int i = 0; i < commandCount; ++i)
	{
		if (commands[i] == cmd)
		{
			(this->*handlers[i])(client, args);
			return;
		}
	}

	const std::string	commands2[] = {
		"USER","NICK"
	};

	int	(Server::*handlers2[])(Client *client, std::vector<std::string> args) = {
	&Server::handleUser,
	&Server::handleNick,
	};

	const int	commandCount2 = sizeof(commands2) / sizeof(commands2[0]);
	for (int i = 0; i < commandCount2; ++i)
	{
		if (commands2[i] == cmd)
		{
			(this->*handlers2[i])(client, args);
			return;
		}
	}

	std::string errMsg = ":" + _network_name + " 421 " + client->getNickname() + cmd + " :Unknown command";
	sendMessage(client->getFd(), errMsg);
}


//<<<<<<<<<<<<<<<<<<<<<<UTILS>>>>>>>>>>>>>>>>>>>>>>>>
void Server::joinGreetings(Client *client)
{
	sendMessage(client->getFd(), Reply::RPL_WELCOME(*client, *this));
	sendMessage(client->getFd(), Reply::RPL_YOURHOST(*this));
	sendMessage(client->getFd(), Reply::RPL_CREATED(*this));
	sendMessage(client->getFd(), Reply::RPL_MYINFO(*client, *this));
}

void	Server::setDateTime()
{
	time_t now = time(0);
	struct tm* local = localtime(&now);

	char buffer[80];
	strftime(buffer, sizeof(buffer), "%H:%M:%S", local);

	std::string timeStr(buffer);
	_StartTime = timeStr;
}

void	signalIgnore()
{
	signal(SIGQUIT, SIG_IGN);
}

void	Server::closeExit()
{
	for (std::map<std::string, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        delete it->second;
    }
	_clients.clear();
	_channels.clear();

	if (_socketfd != -1)
		close(_socketfd);

	for (int i = 0; i <= _nbClients; ++i)
	{
		if (_fds[i].fd != -1)
			close(_fds[i].fd);
	}
}

void	handleSigint(int sig)
{
	(void)sig;
	if (Server::instance)
		Server::instance->closeExit();
	exit(0);
}

void	Server::parsePort(std::string port)
{
	for (size_t i = 0; i < port.size(); ++i) 
		if (!std::isdigit(port[i]))
			std::cerr << "Invalid port\n", exit(1);

	std::stringstream ss(port);
	int	number;
	ss >> number;

	if (number < 1024 || number > 65535)
		std::cerr << "Invalid port\n", exit(1);
	_port = number;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<HELPER METHODS>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

Client*	Server::findClientByFd(int fd)
{
	for (std::map<std::string, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second->getFd() == fd)
			return (it->second);
	}
	return (NULL);
}

Client*	Server::findClientByNick(const std::string& nick)
{	
	if (!_clients.empty())
	{
		std::map<std::string, Client*>::iterator	it = _clients.find(nick);
		return (it != _clients.end() ? it->second : NULL);
	}
	return NULL;
}

Channel*	Server::findChannel(const std::string& name)
{
	std::map<std::string, Channel>::iterator	it = _channels.find(name);
	return (it != _channels.end() ? (&it->second) : NULL);
}

void	Server::sendMessage(int fd, const std::string& message)
{
	std::string msg = message + "\r\n";
	send(fd, msg.c_str(), msg.length(), 0);
}

int Server::findFdIndex(int fd) const
{
    for (int i = 0; i < 1024; ++i)
    {
        if (_fds[i].fd == fd)
            return i;
    }
    return -1; 
}