#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client{
    public:
        Client();
        ~Client();

        std::string getClient() const;
        std::string getNick() const;

    private:
        Client(const Client&);
        Client& operator=(const Client&);

        std::string client;
        std::string nick;
        std::string pass;
};

#endif