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

struct ReturnValues
{
    std::string http;
    int code;
};


class ServerConfig
{
    public:
        ServerConfig();
        ~ServerConfig() {}

        std::vector<std::string>     location;
        ListenValues                 *listen;
        std::string                  serverName;
        std::string                  root;
        std::vector<std::string>     index;
        std::string                  clientMaxBodySize;
        bool                         autoindex;
        std::string                  errorPage;
        ReturnValues                 *_return;

    private:
        bool    built;

};

#endif