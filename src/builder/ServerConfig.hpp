#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include "IConfigBuilder.hpp"
# include "DirectiveProcessor.hpp"
# include <vector>

class ServerConfig
{
    public:
        ServerConfig();
        ~ServerConfig() {}

        std::vector<std::string> location;
        unsigned int    listen;
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