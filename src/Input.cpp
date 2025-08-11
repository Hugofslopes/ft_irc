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

std::vector<std::string> Input::process_args() {
    std::vector<std::string> result;
    std::istringstream iss(_raw);
    std::string line;

    if (std::getline(iss, line)) {
        std::istringstream lineStream(line);
        std::string word;
        while (lineStream >> word) {
            if (!word.empty() && word[0] == '#')
                for (size_t i = 0; i < word.length(); ++i)
                    word[i] = std::tolower(static_cast<unsigned char>(word[i]));
            result.push_back(word);
        }

        size_t eraseLen = line.length();
        if (_raw.length() > eraseLen && (_raw[eraseLen] == '\r' || _raw[eraseLen] == '\n')) {
            eraseLen += (_raw[eraseLen] == '\r' && _raw[eraseLen + 1] == '\n') ? 2 : 1;
        }
        _raw.erase(0, eraseLen);
    }
    return result;
}

std::string Input::getRaw()
{
    return _raw;
}