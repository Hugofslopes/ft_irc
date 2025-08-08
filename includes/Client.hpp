#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <map>
# include <string>
# include <vector>
# include <algorithm>
# include "Input.hpp"
# include "Server.hpp"

class Server;

class Client
{
private:
	std::string					client;
	std::string					username;
	std::string					nick;
	std::string					pass;

	std::string 				_partialMessage;
    std::string 				_buffer;
	Input						_input;

	int							fd;
	bool						registered;

	std::map<std::string, bool>	operatorStatus;
	std::vector<std::string>	channels;

public:
	Client();
	Client(int fd);
	Client& operator=(const Client& other);
	Client(const Client&);
	~Client();

	//INPUT
	Input&			getInput();
    const Input&	getInput() const;
	void			appendPartialMessage(const std::string &message);
	void			clearInput();
	bool			processMessage(const std::string &message);
	bool			processInitialCommands(Server&);

	//Getters
	std::string		getClient() const;
	std::string		getUsername() const;
	std::string		getNickname() const;
	std::string		getPass() const;

	int				getFd() const;

	bool			isRegistered() const;
	bool			isOperator(const std::string& channel) const;

	const std::vector<std::string>&	getChannels() const;

	//Setters
	void	setClient(const std::string& client);
	void	setUsername(const std::string& username);
	void	setNickname(const std::string& nick);
	void	setPass(const std::string& pass);
	void	setRegistered(bool status);
	void	setOperator(const std::string& channel, bool status);

	void	addChannel(const std::string& channel);
	void	removeChannel(const std::string& channel);
};

#endif
