#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>
#include <vector>
#include <sstream>


class Input{
    public:
        Input(std::string);
        Input& operator=(const Input&);
        ~Input();
        Input();

        std::string getCommand() const;

    private:
        Input(const Input&);
        

        std::string                 _command;
        std::string                 _inClient;
        std::vector<std::string>    _args;
};
#endif