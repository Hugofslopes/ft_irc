#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>

class Channel{
    public:
        Channel(std::string);
        ~Channel();

        bool        getKey() const;
        bool        getInvite() const;
        std::string getName() const;
        std::string getTopic() const;

    private:
        Channel();
        Channel(const Channel&);
        Channel& operator=(const Channel&);

        std::string _name;
        std::string _topic;
        bool        _key;
        bool        _invite;
};
#endif