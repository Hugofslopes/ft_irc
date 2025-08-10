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

		void		start();
		void		run();

		static Server*	instance;
		void		closeExit();

		std::string	getName() const;
		std::string	getVersion() const;
		std::string	getStTime() const;
	private:
		Server();
		Server(const Server&);
		Server& operator=(const Server&);

		std::map<std::string, Client *>	_clients;
		std::map<std::string, Channel>	_channels;
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
		void		joinGreetings(Client*);

		Client*		findClientByFd(int);
		Client*		findClientByNick(const std::string&);
		Channel*	findChannel(const std::string&);
		void		sendMessage(int, const std::string&);
		void		removeFromReg(Client *);
		void		processInitialCommands(Client *,std::vector<std::string>);
		void		newClient();
		void		removeClient(Client *, int);
		void		clientRequest(int);
		void		executeCommand(Client *, std::vector<std::string>);
		
		void		processRegister(Client *, std::string);
		void		processRegister2(Client *, std::vector<std::string>);

		void		handleInvite(Client *, std::vector<std::string>);
		void		handleJoin(Client *, std::vector<std::string>);
		void		handleKick(Client *, std::vector<std::string>);
		void		handleMode(Client *, std::vector<std::string>);
		int			handleNick(Client *, std::vector<std::string>);
		void		handlePart(Client *, std::vector<std::string>);
		int			handlePass(Client *, std::vector<std::string>);
		void		handleTopic(Client *, std::vector<std::string>);
		void		handlePrivmsg(Client *, std::vector<std::string>);
		int			handleUser(Client *, std::vector<std::string>);
		void 		handleAllCredenti(Client *, std::vector<std::string>);
		void		handleWho(Client *, std::vector<std::string>);
	};
extern Server*	instance;

void	signalIgnore();
void	handleSigint(int);
# endif
