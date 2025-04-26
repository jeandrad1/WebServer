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

IConfig *ServerBuilder::build(AConfigBlock *serverBlock)
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
    this->server->_return = new t_return;

    t_listen *listen = new t_listen;
    listen->port = DEFAULT_LISTEN_PORT;
    listen->ip = DEFAULT_LISTEN_IP;
    this->server->listen.push_back(listen);
    this->server->listenDirective = false;

    this->server->root = "-1";
    this->server->autoindex = DEFAULT_AUTOINDEX;
	this->server->clientMaxBodySize = -1;

    this->server->serverNames.push_back("");

    this->server->index.push_back(" ");

    this->server->_return->returnDirective = false;
    this->server->errorPageDirective = false;
}

/***********************************************************************/
/*                          Handle Functions                           */
/***********************************************************************/

void    ServerBuilder::handleListen(const std::string &value)
{
    if (this->server->listenDirective == false)
    {
        delete this->server->listen[0];
        this->server->listen.clear();
    }

    this->server->listenDirective = true;

    t_listen    *listen = new t_listen;

    std::string real_value = value.substr(0, value.size() - 1);

    int colon_pos = real_value.find(":");

    if(colon_pos != std::string::npos)
    {
        listen->ip = real_value.substr(0, colon_pos);
        std::string port_str = real_value.substr(colon_pos + 1);
        listen->port = std::atoi(port_str.c_str());
    }
    else
    {
        listen->ip = DEFAULT_LISTEN_IP;
        listen->port = std::atoi(real_value.c_str());
    }
    this->server->listen.push_back(listen);
}

void    ServerBuilder::handleServerName(const std::string &value)
{
    if (this->server->serverNames.size() == 1 && this->server->serverNames[0] == "")
    {
        this->server->serverNames.pop_back();
        this->server->serverNames.clear();
    }

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
    if (this->server->_return->returnDirective == true)
    {
        std::cout << "Error in builder because of Return encountered twice, should throw exception?";
        return ;
    }
    
    this->server->_return->returnDirective = true;

    std::string real_value = value.substr(0, value.size() - 1);
    int http_pos = real_value.find("http");

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
    
    this->server->errorPageDirective = true;

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
    errorPage->targetPage = target.substr(0, target.size() - 1);
    errorPage->isEqualModifier = false;
    errorPage->equalModifier = 0;
    errorPage->referencesCount = 0;

    ite--;
    if (!(*ite).find('='))
    {
        errorPage->isEqualModifier = true;
        errorPage->equalModifier = std::atol((*ite).substr(1, (*ite).size()).c_str());
        ite--;
    }
    ite++;
    for (std::vector<std::string>::iterator it = values.begin(); it != ite; it++)
    {
        errorPage->referencesCount++;
        this->server->errorPages[std::atol((*it).c_str())] = errorPage;
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
    if (this->server->clientMaxBodySize > (1024 * 1024 * 1024))
    this->server->clientMaxBodySize = (1024 * 1024 * 1024);
}
