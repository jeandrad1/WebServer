#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include "IConfigBuilder.hpp"
# include "DirectiveProcessor.hpp"
# include <vector>

typedef struct s_ServerErrorPage
{
    std::string         target;
    std::vector<int> statusCodes;
    bool                isEqualModifier;
    int                 equalModifier;            
}   t_ServerErrorPage;


class LocationConfig;

const int DEFAULT_PORT = -1;
const int DEFAULT_HTTP_CODE = -1;

struct ListenValues
{
    std::string ip;
    int port;
};

struct ServerReturnValues
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
        bool                            *autoindex;
		std::vector<t_ServerErrorPage>        errorPages;
        ServerReturnValues              *_return;
        long long					    clientmaxbodysize;

    private:
        bool    built;

};

#endif