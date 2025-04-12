#ifndef SERVERBUILDER_HPP
# define SERSERVERBUILDER_HPP

# include "IConfigBuilder.hpp"
# include "DirectiveProcessor.hpp"

class ServerBuilder : public IConfigBuilder, public DirectiveProcessor<ServerBuilder>
{
    public:
        ~ServerBuilder() {}

        void    setDirective(const std::string &key,const std::string &value);
        void    addNestedBuilder(ServerBuilder *child);
        void    *build(void);

        void    handleLocation(void);
        void    handleListen(void);
        void    handleServerName(void);
        void    handleRoot(void);
        void    handleIndex(void);
        void    handleClientMaxBodySize(void);
        void    handleAutoindex(void);
        void    handleErrorPage(void);
        void    handleReturn(void)

};

#endif