#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include "IConfigBuilder.hpp"
# include "DirectiveProcessor.hpp"
# include "LocationConfig.hpp"
# include <vector>

const int DEFAULT_PORT = -1;
const int DEFAULT_HTTP_CODE = -1;

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
    ServerConfig()
    {
        listen->port = DEFAULT_PORT;
        _return->code = DEFAULT_HTTP_CODE;
        built = false;       
    }

    ~ServerConfig() {}
    
        std::vector<LocationConfig *>   locations;
        ListenValues                    *listen;
        std::string                     serverName;
        std::string                     root;
        std::vector<std::string>        index;
        std::string                     clientMaxBodySize;
        bool                            autoindex;
        std::string                     errorPage;
        ReturnValues                    *_return;

    private:
        bool    built;

};

#endif