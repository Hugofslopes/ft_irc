#ifndef CHANNEL_HPP
#define CHANNEL_HPP

# include <iostream>
# include <vector>

class Channel{
    public:
        Channel();
        Channel(std::string);
        ~Channel();
        Channel(const Channel&);
        Channel& operator=(const Channel&);

        bool        getKey()const;
        bool        getInvite()const;
        std::string getName()const;
        std::string getTopic()const;
        bool        isOperator(int)const;
        bool        isClient(int)const;
        int         getLimit()const;
        bool        getTopicRestric()const;

        void        addOperator(int);
        void        setTopic(std::string);
        void        setInvite();
        void        setLimit(int);
        void        addClient(int);
        void        setTopicRestric();

    private:
        std::string         _name;
        std::string         _topic;
        bool                _key;
        bool                _invite;
        int                 _limit;
        bool                _topicRestric;
        std::vector<int>    _clients;
        std::vector<int>    _operators;
};
#endif