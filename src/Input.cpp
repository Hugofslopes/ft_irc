#include "../includes/Input.hpp"

Input::Input(){}

Input::~Input(){}

std::string   Input::getFullInput(){
    std::string fullIn;

    for (std::vector<std::string>::iterator it = _args.begin(); it != _args.end(); ++it) {
        fullIn += *it;
    }
    return (fullIn);
}

void  Input::clear(){
    _args.clear();
    _raw.clear();
}

void Input::processInput(std::string in){
        _raw += in;
}

std::vector<std::string> Input::process_args()
{
    std::vector<std::string> tmp;
    std::istringstream iss(_raw);
    std::string word;

    while (iss >> word) {
        _args.push_back(word);
    }

    tmp = _args;
    if (!tmp.empty()) {
        const std::string& last = tmp.back();
        if (last == "\n" || last == "\r\n") {
            tmp.pop_back();
        }
    }
    for (std::vector<std::string>::const_iterator it = 
        tmp.begin(); it != tmp.end(); ++it) {
        std::cout << *it << std::endl;
    }
    clear();
    return tmp;
}

std::string Input::getRaw()
{
    return _raw;
}