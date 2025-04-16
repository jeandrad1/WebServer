#ifndef HTTPCONFIG_HPP
# define HTTPCONFIG_HPP

# include <vector>
# include "ServerConfig.hpp"
# include "structs.hpp"

class HttpConfig {

    public:
        std::vector<ServerConfig *> servers;
        unsigned long               clientMaxBodySize;
        std::vector<t_locationErrorPage>    errorPages;
};

#endif