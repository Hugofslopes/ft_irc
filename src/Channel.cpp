#include "../includes/Channel.hpp"

//<<<<<<<<<<<<<<<<<<<<<<CONSTRUCTORS>>>>>>>>>>>>>>>>>>>>>>>>
Channel::Channel(){}

Channel::Channel(std::string str){
    _name = str;
    _key = false;
    _invite = false;
}

Channel::~Channel(){}

Channel::Channel(const Channel& other){
    _name = other._name;
    _topic = other._topic;
    _key = other._key;
    _invite = other._invite;
}

Channel& Channel::operator=(const Channel& other){
    if (this != &other)
    {
        _name = other._name;
        _topic = other._topic;
        _key = other._key;
        _invite = other._invite;
    }
    return *this;
}

//<<<<<<<<<<<<<<<<<<<<<<GETERS>>>>>>>>>>>>>>>>>>>>>>>>
bool    Channel::getKey()const{
    return _key;
}

bool    Channel::getInvite()const{
    return _invite;
}

std::string Channel::getName()const{
    return _name;
}

std::string Channel::getTopic()const{
    return _topic;
}

bool Channel::isOperator(int index)const{
    std::vector<int>::const_iterator it = _operators.begin();

    for (; it < _operators.end(); it++)
        if (*it == index)
            return true;
    return false;
}

bool Channel::isClient(int index)const{
    std::vector<int>::const_iterator it = _clients.begin();

    for (; it < _clients.end(); it++)
        if (*it == index)
            return true;
    return false;
}

int Channel::getLimit()const{
    return _limit;
}

bool Channel::getTopicRestric()const{
    return _topicRestric;
}

//<<<<<<<<<<<<<<<<<<<<<<SETERS>>>>>>>>>>>>>>>>>>>>>>>>
void Channel::addOperator(int index){
    _operators.push_back(index);
}

void Channel::setTopic(std::string topic){
    _topic = topic;
}

void Channel::setInvite(){
    _invite = true;
}

void Channel::setLimit(int limit){
    _limit = limit;
}

void Channel::addClient(int index){
    _clients.push_back(index);
}

void Channel::setTopicRestric(){
    _topicRestric = true;
}