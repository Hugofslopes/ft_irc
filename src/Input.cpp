#include "../includes/Input.hpp"

Input::Input(std::string str){

    if (str[0] == ':'){
        str.erase(0, 1);
        std::stringstream ss(str);
        std::getline(ss, _inClient, ' ');
        std::getline(ss, _command, ' ');
    }
    else {
        std::stringstream ss(str);
        std::getline(ss, _command, ' ');
    }
}

Input::~Input(){}

std::string Input::getCommand() const{
    return _command;
}

Input::Input(){}

Input::Input(const Input& other){
    _command = other._command;
    _args = other._args;
    _command = other._command;
}

Input& Input::operator=(const Input& other){
    if (this != &other)
    {
        _command = other._command;
        _args = other._args;
        _command = other._command;
    }
    return *this;
}
