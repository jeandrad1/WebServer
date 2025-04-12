#ifndef SERVERBUILDER_HPP
# define SERSERVERBUILDER_HPP

# include "IConfigBuilder.hpp"

class ServerBuilder : public IConfigBuilder
{
    public:
        ~ServerBuilder() {}

        void    setDirective(const std::string &key,const std::string &value);
        void    addNestedBuilder(ServerBuilder *child);
        void    *build(void);
/*
        this->validDirectives.push_back("location");
        this->validDirectives.push_back("listen");
        this->validDirectives.push_back("server_name");
        this->validDirectives.push_back("root");
        this->validDirectives.push_back("index");
        this->validDirectives.push_back("client_max_body_size");
        this->validDirectives.push_back("autoindex");
        this->validDirectives.push_back("error_page");
        this->validDirectives.push_back("return");
*/

    
};

#endif