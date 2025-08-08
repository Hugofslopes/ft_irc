#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>


class Input{
    private:
    void parseComponents();
    void validateBaseCommand();

    std::string _raw;
    std::string _prefix;
    std::string _command;
    std::string _nick;
	std::string _realname;
    std::string _channel;
    std::string _target;
    std::string _key;
    std::string _comment;
	std::string _type;
	std::string _pass;
    std::string _trailing;

    bool		_isValid;
    
    public:
    Input();
    ~Input();

    //PARSE BASES
    void	parse(const std::string &complete_message);
    void	clear();

	//PARSE COMMANDS
	void	parseJoin(std::istringstream &iss);
	void	parseInvite(std::istringstream &iss);
	void	parseKick(std::istringstream &iss);
	void	parseMode(std::istringstream &iss);
	void	parsePass(std::istringstream &iss);
	void	parseNick(std::istringstream &iss);
	void	parseUser(std::istringstream &iss);
	void	parsePart(std::istringstream &iss);
	void	parsePrivmsg(std::istringstream &iss);
	void	parseTopic(std::istringstream &iss);

    //GETTERS
    const std::string	&getPrefix() const;
    const std::string	&getCommand() const;
	const std::string	&getTrailing() const;
    bool				isValid() const;
    const std::string	&getRaw() const;

	//Verificacao de mensagens parciais
	bool isComplete(const std::string &message) const;
};
#endif
