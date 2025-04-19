#include "ServerBuilder.hpp"

// this function is in utils
std::vector<std::string> split_str(const std::string &str, const std::string &delimiter);

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

ServerBuilder::ServerBuilder() : built(false), server(new ServerConfig())
{
    setDefaultValues();

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
    for (size_t i = 0; i < this->server->listen.size(); ++i)
        delete this->server->listen[i];

    if (this->server->errorPages.size() != 0)
    {
        for (size_t i = 0; i < this->server->errorPages.size(); ++i)
            delete this->server->errorPages[i];
    }
    if (this->server)
        delete this->server;
}

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

void    ServerBuilder::setDirective(const std::string &name, const std::string &value)
{
    dispatchDirective(name, value);
}

void    ServerBuilder::addNestedBuilder(IConfigBuilder *child, AConfigBlock *newBlock)
{
    LocationConfig *newLocation = static_cast<LocationConfig *>(child->build(newBlock));
    this->server->locations.push_back(newLocation);
}

void *ServerBuilder::build(AConfigBlock *serverBlock)
{
    if (this->built == true)
        return (NULL);

    for (AConfigBlock::iterator blockIt = serverBlock->begin(); blockIt != serverBlock->end(); ++blockIt)
    {
        if (Directive *directive = dynamic_cast<Directive *>(*blockIt))
            this->dispatchDirective(directive->getName(), directive->getValue());
        else
        {
            LocationBuilder *locationBuilder = new LocationBuilder((*blockIt)->getName());
            this->addNestedBuilder(locationBuilder, *blockIt);
            delete locationBuilder;
        }
    }
    this->built = true;
    return (this->server);
}

void    ServerBuilder::setDefaultValues()
{
    this->server->_return = new t_return();

    t_listen *defaultListen = new t_listen();
    defaultListen->ip = "0.0.0.0";
    defaultListen->port = 80;
    this->server->listen.push_back(defaultListen);

    this->server->serverNames.push_back("");

    if (this->server->root.empty())
        this->server->root = "/";

    if (this->server->index.empty())
        this->server->index.push_back("index.html");

    if (this->server->_return->code == -1)
        this->server->_return->code = 200;

    if (this->server->_return->http.empty())
        this->server->_return->http = "example.com";
}

/***********************************************************************/
/*                          Handle Functions                           */
/***********************************************************************/

void    ServerBuilder::handleListen(const std::string &value)
{
    std::string real_value = value.substr(0, value.size() - 1);

    int colon_pos = real_value.find(":");

    t_listen *newListen = new t_listen();

    if(colon_pos != std::string::npos)
    {
        newListen->ip = real_value.substr(0, colon_pos);
        std::string port_str = real_value.substr(colon_pos + 1);
        newListen->port = std::atoi(port_str.c_str());
    }
    else
    {
        newListen->ip = "";
        newListen->port = std::atoi(real_value.c_str());
    }
}

void    ServerBuilder::handleServerName(const std::string &value)
{
    std::string real_value = value.substr(0, value.size() - 1);
    this->server->serverNames.push_back(real_value);
}

void    ServerBuilder::handleRoot(const std::string &value)
{
    std::string real_value = value.substr(0, value.size() - 1);
    this->server->root = real_value;
}

void    ServerBuilder::handleAutoindex(const std::string &value)
{
    std::string real_value = value.substr(0, value.size() - 1);
    if (real_value == "off")
        this->server->autoindex = false;
    else
        this->server->autoindex = true;
}

void    ServerBuilder::handleReturn(const std::string &value)
{
    std::string real_value = value.substr(0, value.size() - 1);
    int http_pos = real_value.find("h");

    if(http_pos != std::string::npos)
    {
        this->server->_return->http = real_value.substr(http_pos, value.size());
        std::string port_str = real_value.substr(0, http_pos);
        this->server->_return->code = std::atoi(port_str.c_str());
    }
    else
    {
        std::string true_value = real_value.substr(0, std::string::npos);
        this->server->_return->code = std::atoi(true_value.c_str());
    }
}

void    ServerBuilder::handleIndex(const std::string &value)
{
    std::string real_value = value.substr(0, value.size() - 1);
    std::vector<std::string> index = split_str(real_value, " ");
    this->server->index = index;
}

void    ServerBuilder::handleErrorPage(const std::string &value)
{
    std::istringstream iss(value);
    std::vector<std::string> values;
    std::string info;
    t_errorPage *errorPage = new t_errorPage;
    
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

    errorPage->targetPage = target.substr(0, target.size() - 1);
    errorPage->isEqualModifier = false;
    errorPage->equalModifier = 0;

    for (std::vector<std::string>::iterator it = values.begin(); it != ite; it++)
    {
        if ((*it).find('='))
        {
            errorPage->isEqualModifier = true;
            errorPage->equalModifier = std::atol((*it).substr(1, (*it).size()).c_str());
            break ;
        }
        int errorCode = std::atoi((*it).c_str());
        this->server->errorPages[errorCode] = errorPage;
    }
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
			this->server->clientMaxBodySize = maxBodySize * 1024;
			break ;
		case 'm':
			this->server->clientMaxBodySize = maxBodySize * 1024 * 1024;
			break ;
		case 'g':
			this->server->clientMaxBodySize = maxBodySize * 1024 * 1024 * 1024;
			break ;
		case '\0':
			this->server->clientMaxBodySize = maxBodySize;
	}
}
