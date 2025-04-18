#ifndef HTTPCONFIG_HPP
# define HTTPCONFIG_HPP

# include "ServerBuilder.hpp"

class HttpConfig {

    public:
        std::vector<ServerConfig *> servers;

        unsigned long               clientMaxBodySize;
        std::map<int, t_errorPage *>  errorPages;
        bool				        errorPageDirective;
};

#endif