#include "../includes/Irc.hpp"

namespace Irc{
    std::vector<Client>     clients;
    std::vector<Channel>    channels;
    Input                   input;
    std::string             network_na = "ft_irc";
    std::string             version = "1.0";
    std::time_t       const now = std::time(0);
    std::tm*          const tmPtr = std::localtime(&now);

    std::string getDateTime(){
        std::string str;
        char buffer[16];
        
        std::strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", tmPtr);
        str = buffer;
        
        return(str);
    }
}