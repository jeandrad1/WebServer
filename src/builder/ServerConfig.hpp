#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include "IConfigBuilder.hpp"
# include "DirectiveProcessor.hpp"
# include <vector>

class ServerConfig : public IConfigBuilder, public DirectiveProcessor<ServerConfig>
{
    public:
        ~ServerConfig() {}

        void    setDirective(const std::string &key,const std::string &value);
        void    addNestedBuilder(ServerConfig *child);
        void    *build(void);

        std::vector<std::string> location;
        int listen;
        std::string serverName;
        std::string root;
        std::string index;
        std::string clientMaxBodySize;
        bool autoindex;
        std::string errorPage;
        std::string _return;

    private:
        bool    built;

};

#endif