#include "../includes/Server.hpp"

void removeClientByFd(std::map<std::string, Client*>& clients, int fdToRemove) {
    for (std::map<std::string, Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->second && it->second->getFd() == fdToRemove) {
            delete it->second;
            clients.erase(it);
            break;
        }
    }
}

void Server::removeFromReg(Client *client){
	std::string str = "Invalid parameters for regist!! \nPlease reconnect and insert :\"CAP LS 302\"\nPASS <pass_value>\nNICK <nick>\nUSER <user>\n";
	sendMessage(client->getFd(), str);
	removeClientByFd(_clients, client->getFd());
	close(client->getFd());
	_fds[client->getFd()].fd = -1;
}

void	Server::processRegister2(Client *client, std::vector<std::string> args){
	if (client->getStartReg())
		processInitialCommands(client, args);
	else if (!client->getStartReg() && args[0] == "CAP" &&  args[1] == "LS" && args[2] == "302" 
	&& args.size() == 3){
		client->setStartReg(true);
		sendMessage(client->getFd(),"CAP * LS");
		return;
	}
	else
		removeFromReg(client);
}

void	Server::processRegister(Client *client, std::string msg) {
	std::vector<std::string> args;
	client->_input.processInput(msg);
	if (client->_input.getRaw().find("\n") != std::string::npos){
		while (!client->_input.getRaw().empty()){
			args = client->_input.process_args();
			processRegister2(client, args);
		}
	}
	else
		return;
}

void Server::processInitialCommands(Client *client, std::vector<std::string> args)
{
	/* if (args[0] == "CAP" && args[1] == "END")
	{
		if (!client->getNickname().empty() && !client->getPass().empty() 
		&& !client->getUsername().empty() && !client->isRegistered())
		{
			client->setRegistered(true);
			joinGreetings(client);
		}
	} */
	if (args[0] == "PASS" && client->getPass().empty())
		handlePass(client, args);
	else if (args[0] == "NICK" && !client->getPass().empty())
		handleNick(client, args);
	else if (args[0] == "USER" && !client->getNickname().empty())
		handleUser(client, args);
	if (!client->getNickname().empty() && !client->getPass().empty() 
		&& !client->getUsername().empty() && !client->isRegistered())
		{
			client->setRegistered(true);
			joinGreetings(client);
		}
}