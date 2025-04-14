#include "ServerBuilder.hpp"
#include <algorithm>

ServerBuilder::ServerBuilder()
{
    registerHandler("listen", &ServerBuilder::handleListen);
    registerHandler("server_name", &ServerBuilder::handleServerName);
    registerHandler("root", &ServerBuilder::handleRoot);
    registerHandler("index", &ServerBuilder::handleIndex);
    registerHandler("client_max_body_size", &ServerBuilder::handleClientMaxBodySize);
    registerHandler("autoindex", &ServerBuilder::handleAutoindex);
    registerHandler("error_page", &ServerBuilder::handleErrorPage);
    registerHandler("return", &ServerBuilder::handleReturn);
}

void *ServerBuilder::build()
{
    if (this->ServerConfig->listen->ip.empty())
        this->ServerConfig->listen->ip = "42.42.42.42";
    if (this->ServerConfig->listen->port == NULL)
        this->ServerConfig->listen->port = 42;
    if (this->ServerConfig->serverName.empty())
        this->ServerConfig->serverName = "http://example.com";
    if (this->ServerConfig->root.empty())
        this->ServerConfig->root = "/";
    if (this->ServerConfig->listen->port == NULL)
        this->ServerConfig->listen->port = 42;
    if (this->ServerConfig->autoindex == NULL)
        this->ServerConfig->autoindex = false;
    if (this->ServerConfig->_return->code == NULL)
        this->ServerConfig->_return->code = 200;
    if (this->ServerConfig->_return->http.empty())
        this->ServerConfig->_return->http = "example.com";
}

void    ServerBuilder::handleListen(const std::string &value)
{
    int colon_pos = value.find(":");

    if(colon_pos != std::string::npos)
    {
        this->ServerConfig->listen->ip = value.substr(0, colon_pos);
        std::string port_str = value.substr(colon_pos + 1);
        this->ServerConfig->listen->port = std::atoi(port_str.c_str());
    }
    else
    {
        this->ServerConfig->listen->ip = "";
        this->ServerConfig->listen->port = std::atoi(value.c_str());
    }
}

void    ServerBuilder::handleServerName(const std::string &value)
{
    this->ServerConfig->serverName = value;
}

void    ServerBuilder::handleRoot(const std::string &value)
{
    this->ServerConfig->root = value;
}

void    ServerBuilder::handleAutoindex(const std::string &value)
{
    if (value == "off")
        this->ServerConfig->autoindex = false;
    else
        this->ServerConfig->autoindex = true;
}

void    ServerBuilder::handleReturn(const std::string &value)
{
    int http_pos = value.find("h");

    if(http_pos != std::string::npos)
    {
        this->ServerConfig->_return->http = value.substr(http_pos, value.size());
        std::string port_str = value.substr(0, http_pos);
        this->ServerConfig->_return->code = std::atoi(port_str.c_str());
    }
    else
    {
        this->ServerConfig->_return->code = std::atoi(value.c_str());
    }
}


