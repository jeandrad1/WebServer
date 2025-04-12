#ifndef HTTPCONFIG_HPP
# define HTTPCONFIG_HPP

# include <vector>

class HttpConfig {

    public:
        std::vector<ServerConfig *> servers;

};

#endif