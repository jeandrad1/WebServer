#include "ServerBuilder.hpp"
#include <algorithm>
#include <string>
#include <sstream>

// Is in utils
std::vector<std::string> split_str(const std::string &str, const std::string &delimiter);

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
    std::string real_value = value.substr(0, value.size() - 1);

    int colon_pos = real_value.find(":");

    if(colon_pos != std::string::npos)
    {
        this->ServerConfig->listen->ip = real_value.substr(0, colon_pos);
        std::string port_str = real_value.substr(colon_pos + 1);
        this->ServerConfig->listen->port = std::atoi(port_str.c_str());
    }
    else
    {
        this->ServerConfig->listen->ip = "";
        this->ServerConfig->listen->port = std::atoi(real_value.c_str());
    }
}

void    ServerBuilder::handleServerName(const std::string &value)
{
    std::string real_value = value.substr(0, value.size() - 1);
    this->ServerConfig->serverName = real_value;
}

void    ServerBuilder::handleRoot(const std::string &value)
{
    std::string real_value = value.substr(0, value.size() - 1);
    this->ServerConfig->root = real_value;
}

void    ServerBuilder::handleAutoindex(const std::string &value)
{
    std::string real_value = value.substr(0, value.size() - 1);
    if (real_value == "off")
        this->ServerConfig->autoindex = false;
    else
        this->ServerConfig->autoindex = true;
}

void    ServerBuilder::handleReturn(const std::string &value)
{
    std::string real_value = value.substr(0, value.size() - 1);
    int http_pos = real_value.find("h");

    if(http_pos != std::string::npos)
    {
        this->ServerConfig->_return->http = real_value.substr(http_pos, value.size());
        std::string port_str = real_value.substr(0, http_pos);
        this->ServerConfig->_return->code = std::atoi(port_str.c_str());
    }
    else
    {
        std::string true_value = real_value.substr(0, std::string::npos);
        this->ServerConfig->_return->code = std::atoi(true_value.c_str());
    }
}

void    ServerBuilder::handleIndex(const std::string &value)
{
    std::string real_value = value.substr(0, value.size() - 1);
    std::vector<std::string> index = split_str(real_value, " ");
    this->ServerConfig->index = index;
}
