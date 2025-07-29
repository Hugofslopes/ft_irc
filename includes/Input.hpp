#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>
#include <vector>
#include <sstream>


class Input{
    public:
        Input(std::string);
        ~Input();

        std::string getCommand() const;

    private:
        Input();
        Input(const Input&);
        Input& operator=(const Input&);

        std::string                 _command;
        std::string                 _inClient;
        std::vector<std::string>    _args;
};
#endif