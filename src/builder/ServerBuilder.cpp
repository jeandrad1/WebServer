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
