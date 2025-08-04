#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <vector>
# include <map>
# include <ctime>
# include <cstdlib>
# include <poll.h>
# include <csignal>
# include <netinet/in.h>
# include <sys/socket.h>
# include <fcntl.h>
# include <cstring>


# include "Client.hpp"
# include "Input.hpp"
# include "Channel.hpp"
# include "Reply.hpp"
# include "Errors.hpp"

class Server{
    public:
        Server(std::string, std::string);
        ~Server();
        
        std::string getName() const;
        std::string getVersion() const;
        std::string getStTime() const;

        void        start();
        void        run();
        void        newClient();	
        void        clientRequest(int);
        void        executeCommand();
        void        process_login();

        void        handleInvite();
        void        handleJoin();
        void        handleKick();
        void        handleMode();
        void        handleNick();
        void        handlePart();
        void        handlePass();
        void        handleTopic();
        void        handlePrivmsg();
        void        handleUser();

        static Server*          instance;
        void        closeExit();

    private:
        Server();
        Server(const Server&);
        Server& operator=(const Server&);

        std::map<std::string, Client>     _clients;
        std::map<std::string, Channel>    _channels;
        Input                   _input;
        std::string             _network_name;
        std::string             _version;
        std::string             _StartTime;
        uint16_t                _port;
        std::string             _password;
        int                     _nbClients;
        struct pollfd			_fds[1024];
        int                     _socketfd;

        void        setDateTime();
        void        parsePort(std::string);
        void        joinGreetings(int);

        Client*     findClientByFd(int fd);
        Client*     findClientByNick(const std::string& nick);
        Channel*    findChannel(const std::string& name);
        void        sendMessage(int fd, const std::string& message);
};
extern Server* instance;

void signalIgnore();
void handleSigint(int);
# endif
