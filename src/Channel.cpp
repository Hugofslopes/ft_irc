#include "../includes/Channel.hpp"

Channel::Channel(std::string str){
    _name = str;
    _key = false;
    _invite = false;
}

Channel::~Channel(){}

bool    Channel::getKey() const{
    return _key;
}

bool    Channel::getInvite() const{
    return _invite;
}

std::string Channel::getName() const{
    return _name;
}

std::string Channel::getTopic() const{
    return _topic;
}

Channel::Channel(){}

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
       