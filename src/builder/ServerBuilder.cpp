#include "ServerBuilder.hpp"
#include "LocationBuilder.hpp"
#include <algorithm>
#include <string>
#include <sstream>

// this function is in utils
std::vector<std::string> split_str(const std::string &str, const std::string &delimiter);

void    ServerBuilder::addNestedBuilder(IConfigBuilder *child)
{
    LocationConfig *newLocation = static_cast<LocationConfig *>(child->build());
    this->serverConfig->locations.push_back(newLocation);
}

ServerBuilder::ServerBuilder() : built(false), serverConfig(new ServerConfig())
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

ServerBuilder::~ServerBuilder()
{
    delete this->serverConfig;
}

void *ServerBuilder::build()
{
    if (this->serverConfig->listen->ip.empty())
        this->serverConfig->listen->ip = "42.42.42.42";
    
    if (this->serverConfig->listen->port == NULL)
        this->serverConfig->listen->port = 42;
    
    if (this->serverConfig->serverName.empty())
        this->serverConfig->serverName = "http://example.com";
    
    if (this->serverConfig->root.empty())
        this->serverConfig->root = "/";

    if (this->serverConfig->index.empty())
        this->serverConfig->index = {"index.html"};

    if (this->serverConfig->listen->port == NULL)
        this->serverConfig->listen->port = 42;
    
    if (this->serverConfig->autoindex == NULL)
        this->serverConfig->autoindex = false;
    
    if (this->serverConfig->_return->code == NULL)
        this->serverConfig->_return->code = 200;
    
    if (this->serverConfig->_return->http.empty())
        this->serverConfig->_return->http = "example.com";
    
    this->built = true;
    return this->serverConfig;
}

void    ServerBuilder::handleListen(const std::string &value)
{
    std::string real_value = value.substr(0, value.size() - 1);

    int colon_pos = real_value.find(":");

    if(colon_pos != std::string::npos)
    {
        this->serverConfig->listen->ip = real_value.substr(0, colon_pos);
        std::string port_str = real_value.substr(colon_pos + 1);
        this->serverConfig->listen->port = std::atoi(port_str.c_str());
    }
    else
    {
        this->serverConfig->listen->ip = "";
        this->serverConfig->listen->port = std::atoi(real_value.c_str());
    }
}

void    ServerBuilder::handleServerName(const std::string &value)
{
    std::string real_value = value.substr(0, value.size() - 1);
    this->serverConfig->serverName = real_value;
}

void    ServerBuilder::handleRoot(const std::string &value)
{
    std::string real_value = value.substr(0, value.size() - 1);
    this->serverConfig->root = real_value;
}

void    ServerBuilder::handleAutoindex(const std::string &value)
{
    std::string real_value = value.substr(0, value.size() - 1);
    if (real_value == "off")
        this->serverConfig->autoindex = false;
    else
        this->serverConfig->autoindex = true;
}

void    ServerBuilder::handleReturn(const std::string &value)
{
    std::string real_value = value.substr(0, value.size() - 1);
    int http_pos = real_value.find("h");

    if(http_pos != std::string::npos)
    {
        this->serverConfig->_return->http = real_value.substr(http_pos, value.size());
        std::string port_str = real_value.substr(0, http_pos);
        this->serverConfig->_return->code = std::atoi(port_str.c_str());
    }
    else
    {
        std::string true_value = real_value.substr(0, std::string::npos);
        this->serverConfig->_return->code = std::atoi(true_value.c_str());
    }
}

void    ServerBuilder::handleIndex(const std::string &value)
{
    std::string real_value = value.substr(0, value.size() - 1);
    std::vector<std::string> index = split_str(real_value, " ");
    this->serverConfig->index = index;
}
