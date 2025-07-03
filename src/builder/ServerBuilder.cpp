#include "ServerBuilder.hpp"

// this function is in utils
std::vector<std::string>	split_str(const std::string &str, const std::string &delimiter);

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

void	ServerBuilder::setDirective(const std::string &name, const std::string &value)
{
	dispatchDirective(name, value);
}

void	ServerBuilder::addNestedBuilder(IConfigBuilder *child, AConfigBlock *newBlock)
{
	LocationConfig	*newLocation = static_cast<LocationConfig *>(child->build(newBlock));
	this->server->addLocation(newLocation);
}

IConfig	*ServerBuilder::build(AConfigBlock *serverBlock)
{
	if (this->built == true)
		return (NULL);

	for (AConfigBlock::iterator	blockIt = serverBlock->begin(); blockIt != serverBlock->end(); ++blockIt)
	{
		if (Directive	*directive = dynamic_cast<Directive *>(*blockIt))
			this->dispatchDirective(directive->getName(), directive->getValue());
		else
		{
			LocationBuilder	*locationBuilder = new LocationBuilder((*blockIt)->getName());
			this->addNestedBuilder(locationBuilder, *blockIt);
			delete locationBuilder;
		}
	}
	this->built = true;
	return (this->server);
}

void	ServerBuilder::setDefaultValues()
{
	t_listen	*listen = new t_listen;
	
	listen->port = DEFAULT_LISTEN_PORT;
	listen->ip = DEFAULT_LISTEN_IP;
	this->server->addListen(listen);
	this->server->setListenDirective(false);
	
	std::string root_value = "-1";
    this->server->setRoot(root_value);
    this->server->setAutoIndex(DEFAULT_AUTOINDEX);
	this->server->setClientMaxBodySize(-1);
	
	this->server->addServerNames("");
	
    this->server->addIndex(" ");
	
	t_return *treturn = new t_return;
	treturn->returnDirective = false;
	this->server->setReturn(treturn);
	
	this->server->setErrorPageDirective(false);
}

/***********************************************************************/
/*                          Handle Functions                           */
/***********************************************************************/

void	ServerBuilder::handleListen(const std::string &value)
{
	if (this->server->getListenDirective() == false)
		this->server->clearListen();
	this->server->setListenDirective(true);

	t_listen	*listen = new t_listen;

	std::string	real_value = value.substr(0, value.size() - 1);
	size_t		colon_pos = real_value.find(":");

	if(colon_pos != std::string::npos)
	{
		listen->ip = real_value.substr(0, colon_pos);
		std::string	port_str = real_value.substr(colon_pos + 1);
		listen->port = std::atoi(port_str.c_str());
	}
	else if (real_value.find(".") != std::string::npos)
	{
		listen->ip = real_value;
		listen->port = DEFAULT_LISTEN_PORT;
	}
	else
	{
		listen->ip = DEFAULT_LISTEN_IP;
		listen->port = std::atoi(real_value.c_str());
	}
	this->server->addListen(listen);
}

void	ServerBuilder::handleServerName(const std::string &value)
{
	if (this->server->getServerNames().size() == 1 && this->server->getServerNames()[0] == "")
		this->server->clearServerName();

	std::string	real_value = value.substr(0, value.size() - 1);
	this->server->addServerNames(real_value);
}

void	ServerBuilder::handleRoot(const std::string &value)
{
	std::string	real_value = value.substr(0, value.size() - 1);
	this->server->setRoot(real_value);
}

void	ServerBuilder::handleAutoindex(const std::string &value)
{
	std::string	real_value = value.substr(0, value.size() - 1);
	if (real_value == "off")
		this->server->setAutoIndex(false);
	else
		this->server->setAutoIndex(true);
}

void	ServerBuilder::handleReturn(const std::string &value)
{
	if (this->server->getReturn()->returnDirective == true)
	{
		std::cout << "Error in builder because of Return encountered twice, should throw exception?";
		return ;
	}

	this->server->setReturnDirective(true);

	std::string	real_value = value.substr(0, value.size() - 1);
	size_t		http_pos = real_value.find("http");

	if (http_pos != std::string::npos)
	{
		std::string httpBuffer = real_value.substr(http_pos, value.size());
		this->server->setReturnHttp(httpBuffer);
		std::string	port_str = real_value.substr(0, http_pos);
		this->server->setReturnCode(std::atoi(port_str.c_str()));
	}
	else
	{
		std::string	true_value = real_value.substr(0, std::string::npos);
		this->server->setReturnCode(std::atoi(true_value.c_str()));
	}
}

void	ServerBuilder::handleIndex(const std::string &value)
{
	std::string					real_value = value.substr(0, value.size() - 1);
	std::vector<std::string>	index = split_str(real_value, " ");

	this->server->setIndex(index);
}

void	ServerBuilder::handleErrorPage(const std::string &value)
{
	std::istringstream			iss(value);
	std::vector<std::string>	values;
	std::string					info;
	t_errorPage					*errorPage = new t_errorPage;

	this->server->setErrorPageDirective(true);

	if (value.empty())
		return ;

	while (!iss.eof())
	{
		iss >> info;
		values.push_back(info);
	}

	std::vector<std::string>::iterator	ite = values.end();
	ite--;
	std::string	target = *ite;

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
	for (std::vector<std::string>::iterator	it = values.begin(); it != ite; it++)
	{
		errorPage->referencesCount++;
		this->server->addErrorPage(std::atoi((*it).c_str()), errorPage);
	}
}

void	ServerBuilder::handleClientMaxBodySize(std::string const &value)
{
	if (value.empty())
		return ;

	std::string			real_value = value.substr(0, value.size() - 1);
	std::istringstream	iss(real_value);
	unsigned long		maxBodySize = 0;
	char				suffix = '\0';

	iss >> maxBodySize;
	iss >> suffix;

	switch (tolower(suffix))
	{
		case 'k':
			this->server->setClientMaxBodySize(maxBodySize * 1024);
			break ;
		case 'm':
			this->server->setClientMaxBodySize(maxBodySize * 1024 * 1024);
			break ;
		case 'g':
			this->server->setClientMaxBodySize(maxBodySize * 1024 * 1024 * 1024);
			break ;
		case '\0':
			this->server->setClientMaxBodySize(maxBodySize);
			break ;
		default :
			std::cout<<"Impossible value found"<<std::endl;
			return ;
	}
	if (this->server->getClientMaxBodySize() > (1024 * 1024 * 1024))
		this->server->setClientMaxBodySize(1024 * 1024 * 1024);
}
