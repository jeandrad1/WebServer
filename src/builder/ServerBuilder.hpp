#ifndef SERVERBUILDER_HPP
# define SERSERVERBUILDER_HPP

# include "IConfigBuilder.hpp"
# include "DirectiveProcessor.hpp"
# include "ServerConfig.hpp"

class ServerBuilder : public IConfigBuilder, public DirectiveProcessor<ServerBuilder>
{
    public:
        // The constructor lacks the handle for location
        ServerBuilder();
        ~ServerBuilder() {}

        void    setDirective(const std::string &key,const std::string &value);
        void    addNestedBuilder(ServerBuilder *child);
        void    *build(void);

        void    handleListen(const std::string &value);
        void    handleServerName(const std::string &value);
        void    handleRoot(const std::string &value);
        void    handleIndex(const std::string &value);
        void    handleClientMaxBodySize(const std::string &value);
        void    handleAutoindex(const std::string &value);
        void    handleErrorPage(const std::string &value);
        void    handleReturn(const std::string &value);

        void    handleLocation(const std::string &value);
        
    private:
        bool            built;
        ServerConfig    *ServerConfig; 

};

#endif