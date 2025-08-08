#ifndef SERVER_HPP
# define SERVER_HPP

# include <cstdint>
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
# include <errno.h>
# include <cstring>
# include <cctype>
# include <cstddef>
# include <sstream>
# include <string>
# include <unistd.h>

# include "Client.hpp"
# include "Input.hpp"
# include "Channel.hpp"
# include "Reply.hpp"
# include "Errors.hpp"

class Server
{
	public:
		Server(std::string, std::string);
		~Server();

<<<<<<< HEAD
        void        start();
        void        run();
        void        newClient();	
        void        clientRequest(int);
        void        executeCommand(int);
        void        process_login();

        void        handleInvite(int);
        void        handleJoin(int);
        void        handleKick(int);
        void        handleMode(int);
        void        handleNick(int);
        void        handlePart(int);
        void        handlePass(int);
        void        handleTopic(int);
        void        handlePrivmsg(int);
        void        handleUser(int);
        
        static Server*          instance;
        void        closeExit();
=======
		std::string	getName() const;
		std::string	getVersion() const;
		std::string	getStTime() const;

		void		start();
		void		run();
		void		newClient();
		void		clientRequest(int);
		void		executeCommand();
		void		process_login();

		void		handleInvite();
		void		handleJoin();
		void		handleKick();
		void		handleMode();
		void		handleNick();
		void		handlePart();
		void		handlePass();
		void		handleTopic();
		void		handlePrivmsg();
		void		handleUser();
>>>>>>> origin/Manuel

		static Server*	instance;
		void		closeExit();

<<<<<<< HEAD
        std::map<std::string, Client>   _clients;
        std::map<std::string, Channel>  _channels;
        Input                           _input;
        std::string                     _network_name;
        std::string                     _version;
        std::string                     _StartTime;
        uint16_t                        _port;
        std::string                     _password;
        int                             _nbClients;
        struct pollfd			        _fds[1024];
        int                             _socketfd;
=======
	private:
		Server();
		Server(const Server&);
		Server& operator=(const Server&);
>>>>>>> origin/Manuel

		std::map<std::string, Client>	_clients;
		std::map<std::string, Channel>	_channels;
		Input							_input;
		std::string						_network_name;
		std::string						_version;
		std::string						_StartTime;
		uint16_t						_port;
		std::string						_password;
		int								_nbClients;
		struct pollfd					_fds[1024];
		int								_socketfd;

		void		setDateTime();
		void		parsePort(std::string);
		void		joinGreetings(int);

		Client*		findClientByFd(int fd);
		Client*		findClientByNick(const std::string& nick);
		Channel*	findChannel(const std::string& name);
		void		sendMessage(int fd, const std::string& message);
};
extern Server*	instance;

void	signalIgnore();
void	handleSigint(int);
# endif
