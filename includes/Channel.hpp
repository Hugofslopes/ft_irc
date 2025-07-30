#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>

class Channel{
    public:
        Channel();
        Channel(std::string);
        ~Channel();
        Channel(const Channel&);
        Channel& operator=(const Channel&);

        bool        getKey() const;
        bool        getInvite() const;
        std::string getName() const;
        std::string getTopic() const;

    private:
        
        std::string _name;
        std::string _topic;
        bool        _key;
        bool        _invite;
};
#endif