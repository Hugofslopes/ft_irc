#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <vector>
# include <ctime>
# include <cstdlib>
# include <poll.h>
# include <csignal>
# include <netinet/in.h>
# include <sys/socket.h>
# include <fcntl.h>
#include <cstring>


# include "Client.hpp"
# include "Input.hpp"
# include "Channel.hpp"

class Server{
    public:
        Server(std::string, std::string);
        ~Server();
        
        std::string getName() const;
        std::string getVersion() const;
        std::string getStTime() const;

        void    start();
        void    run();
        void    newClient();	
	    void    clientRequest(int);
        void    executeCommand();

        void    handleInvite();
        void    handleJoin();
        void    handleKick();
        void    handleMode();
        void    handleNick();
        void    handlePart();
        void    handlePass();
        void    handleTopic();
        void    handlePrivmsg();
        void    handleUser();
        
        
    private:
        Server();
        Server(const Server&);
        Server& operator=(const Server&);

        std::vector<Client>     _clients;
        std::vector<Channel>    _channels;
        Input                   _input;
        std::string             _network_name;
        std::string             _version;
        std::string             _StartTime;
        uint16_t                _port;
        std::string             _password;
        int                     _nbClients;
        struct pollfd			_fds[1024];
        int                     _socketfd;

        void    setDateTime();
        void    parsePort(std::string);
};

void signal_ignore();

# endif