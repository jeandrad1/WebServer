#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include "IConfigBuilder.hpp"
# include "DirectiveProcessor.hpp"
# include <vector>

struct ListenValues
{
    std::string ip;
    int port;
};

class ServerConfig
{
    public:
        ServerConfig();
        ~ServerConfig() {}

        std::vector<std::string> location;
        ListenValues    *listen;
        std::string     serverName;
        std::string     root;
        std::string     index;
        std::string     clientMaxBodySize;
        bool            autoindex;
        std::string     errorPage;
        std::string     _return;

    private:
        bool    built;

};

#endif