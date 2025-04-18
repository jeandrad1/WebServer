#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include "IConfigBuilder.hpp"
# include "DirectiveProcessor.hpp"
# include "LocationConfig.hpp"

typedef struct s_listen
{
    std::string ip;
    int port;
}   t_listen;

class ServerConfig {
    
    public:
        std::vector<LocationConfig *> locations;
        
        std::vector<t_listen *>     listen;
        bool                        listenDirective;
        t_return                    *_return;
        long long                   clientMaxBodySize;
        bool                        autoindex;
        std::string                 root;
        bool				        errorPageDirective;
        std::map<int, t_errorPage *>  errorPages;
        std::vector<std::string>    serverNames;
        std::vector<std::string>    index;
};

#endif