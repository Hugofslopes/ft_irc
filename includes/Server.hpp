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
		void		joinGreetings(int);

		Client*		findClientByFd(int fd);
		Client*		findClientByNick(const std::string& nick);
		Channel*	findChannel(const std::string& name);
		void		sendMessage(int fd, const std::string& message);
		void		removeFromReg(Client *client);
		void		processInitialCommands(Client *client,std::vector<std::string> args);
		void		newClient();
		void		removeClient(Client *client, int index);
		void		clientRequest(int);
		void		executeCommand(Client *client, std::vector<std::string> args);
		
		void		processRegister(Client *client, std::string msg);

		void		handleInvite(Client *client, std::vector<std::string> args);
		void		handleJoin(Client *client, std::vector<std::string> args);
		void		handleKick(Client *client, std::vector<std::string> args);
		void		handleMode(Client *client, std::vector<std::string> args);
		void		handleNick(Client *client, std::vector<std::string> args);
		void		handlePart(Client *client, std::vector<std::string> args);
		void		handlePass(Client *client, std::vector<std::string> args);
		void		handleTopic(Client *client, std::vector<std::string> args);
		void		handlePrivmsg(Client *client, std::vector<std::string> args);
		void		handleUser(Client *client, std::vector<std::string> args);
		void 		handleAllCredenti(Client *client, std::vector<std::string> args);
	};
extern Server*	instance;

void	signalIgnore();
void	handleSigint(int);
# endif
