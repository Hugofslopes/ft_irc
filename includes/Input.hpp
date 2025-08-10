#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>


class Input{
    private:
        std::vector<std::string>    _args;
        std::string                 _raw;

    public:
        Input();
        ~Input();

        std::string                 getFullInput();
        void                        clear();
        std::string                 getRaw();
        void                        processInput(std::string);
        std::vector<std::string>    process_args();
};
#endif
