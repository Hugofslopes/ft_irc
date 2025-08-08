#include "../includes/Input.hpp"

Input::Input() : _isValid(false) {}
Input::~Input(){}

//FUNCAO PRINCIPAL DE PARSING
void Input::parse(const std::string &complete_message) {
    clear();
    _raw = complete_message;
    parseComponents();
    validateBaseCommand();
}

//da clear a tudo
void Input::clear() {
    _raw.clear();
    _prefix.clear();
    _command.clear();
    _nick.clear();
    _realname.clear();
    _channel.clear();
    _target.clear();
    _key.clear();
    _comment.clear();
    _type.clear();
    _pass.clear();
    _trailing.clear();
    _isValid = false;
}

void Input::parseComponents() {
    std::string message = _raw;

    //Limpar \r\n -> versao atualizada cpp98
    while (!message.empty() && (message[message.size() - 1] == '\r' || message[message.size() - 1] == '\n'))
        message.erase(message.size() - 1);

    //Verificar ':' e indicar como prefixo!!
    if (!message.empty() && message[0] == ':')
	{
        size_t prefix_end = message.find(' ');
        if (prefix_end != std::string::npos)
		{
            _prefix = message.substr(1, prefix_end - 1);
            message = message.substr(prefix_end + 1);
        }
    }

    //PARSING
    std::istringstream iss(message);
    std::string token;

    if (iss >> token)
	{
        std::transform(token.begin(), token.end(), token.begin(), ::toupper);
        _command = token;
    }
    if (_command == "JOIN")
		parseJoin(iss);
    else if (_command == "INVITE")
		parseInvite(iss);
    else if (_command == "KICK")
		parseKick(iss);
    else if (_command == "MODE")
		parseMode(iss);
    else if (_command == "PASS")
		parsePass(iss);
    else if (_command == "NICK")
		parseNick(iss);
    else if (_command == "USER")
		parseUser(iss);
    else if (_command == "PART")
		parsePart(iss);
    else if (_command == "PRIVMSG")
		parsePrivmsg(iss);
    else if (_command == "TOPIC")
		parseTopic(iss);

	//Dar handle de trailing das mensagens todas
    std::string trailing;

    if (std::getline(iss, trailing))
	{
        if (!trailing.empty() && trailing[0] == ':')
            _trailing = trailing.substr(1);
        else if (!trailing.empty())
            _trailing = trailing;
    }
}

//JOIN - <channel> [key]
void Input::parseJoin(std::istringstream &iss) {
    iss >> _channel;
	if (iss)
		iss >> _key;
}

//INVITE - <nick> <channel>
void Input::parseInvite(std::istringstream &iss) {
    iss >> _nick;
    iss >> _channel;
}

//KICK - <channel> <user> [comment]
void Input::parseKick(std::istringstream &iss) {
    iss >> _channel;
    iss >> _nick;
	//se existir comentario
	if (iss) {
		std::getline(iss, _comment);
		if (!_comment.empty() && _comment[0] == ':') {
			_comment = _comment.substr(1);
		}
	}
}

//MODE - <target> [type]
void Input::parseMode(std::istringstream &iss) {
    iss >> _target;
    iss >> _type; //+i, +o, e essa palhacada toda...
    if (_type.find('k') != std::string::npos || _type.find('l') != std::string::npos)
		iss >> _key; //password ou limite
    else if (_type.find('o') != std::string::npos)
		iss >> _nick; //nick
}

//PASS - <password>
void Input::parsePass(std::istringstream &iss) { iss >> _pass; }

//NICK - <nick>
void Input::parseNick(std::istringstream &iss) { iss >> _nick; }

//USER - <user> 0 * <realname>
void Input::parseUser(std::istringstream &iss) {
    iss >> _nick; //username aqui!

	//Pular os campos 0 e *, e pegar o realname
    std::string dummy;
    iss >> dummy >> dummy;
	iss >> _realname;
}

//PART - <channel> [reason]
void Input::parsePart(std::istringstream& iss) {
    iss >> _channel;
    std::getline(iss, _comment); // Opcional
    if (!_comment.empty() && _comment[0] == ':') {
        _comment = _comment.substr(1);
    }
}

//PRIVMSG - <target> <text>
void Input::parsePrivmsg(std::istringstream& iss) { iss >> _target;}

//TOPIC - <channel> [topic]
void Input::parseTopic(std::istringstream& iss) {
    iss >> _channel;
    std::getline(iss, _trailing); // Opcional
    if (!_trailing.empty() && _trailing[0] == ':') {
        _trailing = _trailing.substr(1);
    }
}

//Talvez seja repetitivo... verificado na execucao??
//Verifica se o _command existe
void Input::validateBaseCommand() {
    static const std::string valid_commands[] = {
        "NICK", "USER", "JOIN", "PART", "MODE", "TOPIC", "NAMES",
        "LIST", "INVITE", "KICK", "PRIVMSG", "NOTICE", "QUIT", "PASS"
    };

    _isValid = false;
    if (_command.empty())
		return;
	
	const size_t num_commands = sizeof(valid_commands) / sizeof(valid_commands[0]);
	
    for (size_t i = 0; i < num_commands; ++i) {
        if (_command == valid_commands[i]) {
            _isValid = true;
            break;
        }
    }
}

//Verificar se a mensagem nao e parcial
bool Input::isComplete(const std::string &message) const {
    return (message.size() >= 2 &&
		message[message.size()-2] == '\r'
		&& message[message.size()-1] == '\n');
}

//Getters
const std::string& Input::getPrefix() const { return _prefix; }
const std::string& Input::getCommand() const { return _command; }
const std::string& Input::getTrailing() const { return _trailing; }
const std::string& Input::getRaw() const { return _raw; }
bool Input::isValid() const { return _isValid; }
