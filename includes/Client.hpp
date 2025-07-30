#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client{
    public:
        Client();
        ~Client();
        Client& operator=(const Client&);
        Client(const Client&);

        std::string getClient() const;
        std::string getNick() const;
        

    private:
        std::string client;
        std::string nick;
        std::string pass;
};

#endif