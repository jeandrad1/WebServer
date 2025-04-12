#ifndef SERVERBUILDER_HPP
# define SERSERVERBUILDER_HPP

# include "IConfigBuilder.hpp"
# include "DirectiveProcessor.hpp"

class ServerBuilder : public IConfigBuilder, public DirectiveProcessor<ServerBuilder>
{
    public:
        // Constructor must call registerhandle
        ServerBuilder();
        ~ServerBuilder() {}

        void    setDirective(const std::string &key,const std::string &value);
        void    addNestedBuilder(ServerBuilder *child);
        void    *build(void);

        void    handleLocation(std::string value);
        void    handleListen(std::string value);
        void    handleServerName(std::string value);
        void    handleRoot(std::string value);
        void    handleIndex(std::string value);
        void    handleClientMaxBodySize(std::string value);
        void    handleAutoindex(std::string value);
        void    handleErrorPage(std::string value);
        void    handleReturn(std::string value);

    private:
        bool    built;

};

#endif