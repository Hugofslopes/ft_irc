#include "../includes/Server.hpp"

void Server::removeFromReg(Client *client){
	std::string str = "Invalid parameters for regist!! \n\
	Please reconnect and insert : \"CAP LS 302\"\nPASS <pass_value>\n\
	NICK <nick>\nUSER <user>\n";
	sendMessage(client->getFd(), str);
	
	std::string nick = client->getNickname();
	std::map<std::string, Client*>::iterator it = _clients.find(nick);
	if (it != _clients.end()) {
		delete it->second;
		_clients.erase(it);
	}
	close(client->getFd());
	_fds[client->getFd()].fd = -1;
}

bool checkHexIn(std::vector<std::string> args){
	bool Pass = false, Nick = false, User =false;
	for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it)
		if (*it == "PASS")
			Pass = true;
	for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it)
		if (*it == "NICK")
			Nick = true;
	for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it)
		if (*it == "USER")
			User = true;
	if (Pass && Nick && User)
		return true;
	return false;
}

void	Server::processRegister(Client *client, std::string msg) {
	std::vector<std::string> args;
	client->_input.processInput(msg);
	if (client->_input.getRaw().find("\n") != std::string::npos)
		args = client->_input.process_args();
	else
		return;
	if (!client->getStartReg()){
		if (args[0] == "CAP" &&  args[1] == "LS" && args[2] == "302"
		&& args[3] == "PASS"){
			client->setStartReg(true);
			sendMessage(client->getFd(),":ft_irc CAP * LS");
			if (args.size() >= 3) 
				args.erase(args.begin(), args.begin() + 3);
			if (checkHexIn(args))
				handleAllCredenti(client, args);
			return;
		}
		else if ((args[0] == "CAP" &&  args[1] == "LS" && args[2] == "302") 
		&& args.size() == 3){
			client->setStartReg(true);
			sendMessage(client->getFd(),":ft_irc CAP * LS");
			return;
		}
		else
			removeFromReg(client);
	}
	if (client->getStartReg())
		processInitialCommands(client, args);
}

void Server::handleAllCredenti(Client *client, std::vector<std::string> args){
	std::vector<std::string> vec;
	for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it) {
		if (*it == "PASS")
		{
			std::vector<std::string> tmp;
			tmp.push_back(*it);
			it++;
			tmp.push_back(*it);
			it++;
			handlePass(client, tmp);
		}
		if (*it == "NICK")
		{
			std::vector<std::string> tmp;
			tmp.push_back(*it);
			it++;
			tmp.push_back(*it);
			it++;
			handleNick(client, tmp);
		}
		vec.push_back(*it);
	}
	handleUser(client, vec);
}

void Server::processInitialCommands(Client *client, std::vector<std::string> args)
{
	if (args[0] == "CAP" && args[1] == "END")
	{
		if (!client->getNickname().empty() && !client->getNickname().empty() && !client->getNickname().empty())
		{
			client->setRegistered(true);
			joinGreetings(client->getFd());
		}
	}
	else if (args[0] != "PASS" && client->getPass().empty())
		removeFromReg(client);
	else if (args[0] == "PASS")
		handlePass(client, args);
	else if (args[0] == "NICK")
		handleNick(client, args);
	else if (args[0] == "USER" && !client->getNickname().empty())
		handleUser(client, args);
}

/* PASS yourPassword

NICK yourNickname
USER yourUsername 0 * :Real Name */

/* :you!user@host JOIN #coolchannel
:irc.example.net MODE #coolchannel +nt
:irc.example.net 353 you = #coolchannel :you Alice Bob
:irc.example.net 366 you #coolchannel :End of /NAMES list.
 */