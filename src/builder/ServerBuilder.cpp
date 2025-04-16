#include "ServerBuilder.hpp"
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>

// this function is in utils
std::vector<std::string> split_str(const std::string &str, const std::string &delimiter);

void    ServerBuilder::setDirective(const std::string &name, const std::string &value)
{
    dispatchDirective(name, value);
}

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
    if (this->serverConfig)
        delete this->serverConfig;
}

void *ServerBuilder::build()
{
    if (this->serverConfig->listen->ip.empty())
        this->serverConfig->listen->ip = "42.42.42.42";
    
    if (this->serverConfig->listen->port == -1)
        this->serverConfig->listen->port = 42;
    
    if (this->serverConfig->serverName.empty())
        this->serverConfig->serverName = "http://example.com";
    
    if (this->serverConfig->root.empty())
        this->serverConfig->root = "/";

    if (this->serverConfig->index.empty())
        this->serverConfig->index.push_back("index.html");
    
    if (this->serverConfig->listen->port == -1)
        this->serverConfig->listen->port = 42;
    
    if (this->serverConfig->autoindex == NULL)
        this->serverConfig->autoindex = 0;
    
    if (this->serverConfig->_return->code == -1)
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
        *(this->serverConfig->autoindex) = false;
    else
       *(this->serverConfig->autoindex) = true;
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

void    ServerBuilder::handleErrorPage(const std::string &value)
{
    std::istringstream iss(value);
    std::vector<std::string> values;
    std::string info;
    t_ServerErrorPage errorPage;
    
    if (value.empty())
    return ;
    
    while (!iss.eof())
    {
        iss >> info;
        values.push_back(info);
    }
    
    std::vector<std::string>::iterator ite = values.end();
    ite--;
    std::string target = *ite;

    errorPage.target = target.substr(0, target.size() - 1);
    errorPage.isEqualModifier = false;
    errorPage.equalModifier = 0;

    for (std::vector<std::string>::iterator it = values.begin(); it != ite; it++)
    {
        if ((*it).find('='))
        {
            errorPage.isEqualModifier = true;
            errorPage.equalModifier = std::atol((*it).substr(1, (*it).size()).c_str());
            break ;
        }
        errorPage.statusCodes.push_back(std::atol((*it).c_str()));
    }
    this->serverConfig->errorPages.push_back(errorPage);
}

void ServerBuilder::handleClientMaxBodySize(std::string const &value)
{
	if (value.empty())
		return ;

	std::istringstream	iss(value);
	unsigned long		maxBodySize = 0;
	char				suffix = '\0';

	iss >> maxBodySize;
	iss >> suffix;
	switch (tolower(suffix))
	{
		case 'k':
			this->serverConfig->clientmaxbodysize = maxBodySize * 1024;
			break ;
		case 'm':
			this->serverConfig->clientmaxbodysize = maxBodySize * 1024 * 1024;
			break ;
		case 'g':
			this->serverConfig->clientmaxbodysize = maxBodySize * 1024 * 1024 * 1024;
			break ;
		case '\0':
			this->serverConfig->clientmaxbodysize = maxBodySize;
	}
}

