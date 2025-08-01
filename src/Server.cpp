#include "../includes/Server.hpp"
#include "../includes/Input.hpp"

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
    Client client;
    //Client client(_nbClients);
    _clients.push_back(client);
}

void    Server::clientRequest(int index){
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    int bytesRead = recv(_fds[index].fd, buffer, sizeof(buffer) - 1, 0);
    std::cout << "BYTESREAD- " << bytesRead << std::endl;
    if (bytesRead <= 0) {
        close(_fds[index].fd);
        _fds[index].fd = -1;
        std::cout << "Client disconnected\n";
        return;
    }

    std::string message(buffer, bytesRead);
    std::cout << "MESSAGE- " << message << std::endl;
    _input = Input(message);

    /* if (!(_clients[index].getLogin()))
        process_login();
    else
        executeCommand(); */
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
}

void    Server::process_login(){}

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

//<<<<<<<<<<<<<<<<<<<<<<EXECUTE COMMANDS>>>>>>>>>>>>>>>>>>>>>>>>

void    Server::handleInvite(){
    std::cout << "INVITE" << std::endl;
}

void    Server::handleJoin(){}

void    Server::handleKick(){}

void    Server::handleMode(){}

void    Server::handleNick(){}

void    Server::handlePart(){}

void    Server::handlePass(){}

void    Server::handleTopic(){}

void    Server::handlePrivmsg(){}

void    Server::handleUser(){}