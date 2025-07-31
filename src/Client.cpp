#include "../includes/Client.hpp"

Client::Client(){
    client = "default";
    nick = "default";
}

Client::~Client(){

}

std::string Client::getClient() const{
    return client;
}

std::string Client::getNick() const{
    return nick;
}

Client::Client(const Client& other){
    client = other.client;
}

Client& Client::operator=(const Client& other){
    if (this != &other)
    {
        client = other.client;
    }
    return *this;
}
