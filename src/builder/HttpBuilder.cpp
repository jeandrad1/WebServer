#include "HttpBuilder.hpp"

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

HttpBuilder::HttpBuilder() : built(false), http(new HttpConfig())
{
	setDefaultValues();

	registerHandler("client_max_body_size", &HttpBuilder::handleClientMaxBodySize);
	registerHandler("error_page", &HttpBuilder::handleErrorPage);
}

HttpBuilder::~HttpBuilder()
{

}

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

void	HttpBuilder::setDirective(const std::string &name, const std::string &value)
{
	dispatchDirective(name, value);
}

void	HttpBuilder::addNestedBuilder(IConfigBuilder *child, AConfigBlock *newBlock)
{
	ServerConfig	*newServer = static_cast<ServerConfig *>(child->build(newBlock));
	http->servers.push_back(newServer);
}

IConfig	*HttpBuilder::build(AConfigBlock *httpBlock)
{
	for (AConfigBlock::iterator	blockIt = httpBlock->begin(); blockIt != httpBlock->end(); ++blockIt)
	{
		if (Directive *directive = dynamic_cast<Directive *>(*blockIt))
			this->dispatchDirective(directive->getName(), directive->getValue());
		else
		{
			ServerBuilder *serverBuilder = new ServerBuilder();
			this->addNestedBuilder(serverBuilder, *blockIt);
			delete (serverBuilder);
		}
	}
	this->built = true;
	return (this->http);
}

void	HttpBuilder::setDefaultValues(void)
{
	this->http->clientMaxBodySize = DEFAULT_CLIENT_MAX_BODY_SIZE;
    this->http->clientMaxBodySize = -1;
}

/***********************************************************************/
/*                          Handle Functions                           */
/***********************************************************************/

void	HttpBuilder::handleClientMaxBodySize(const std::string &value)
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
			this->http->clientMaxBodySize = maxBodySize * 1024;
			break ;
		case 'm':
			this->http->clientMaxBodySize = maxBodySize * 1024 * 1024;
			break ;
		case 'g':
			this->http->clientMaxBodySize = maxBodySize * 1024 * 1024 * 1024;
			break ;
		case '\0':
			this->http->clientMaxBodySize = maxBodySize;
			break ;
		default :
			std::cout<<"Impossible value found"<<std::endl;
			return ;
	}
	if (this->http->clientMaxBodySize > (1024 * 1024 * 1024))
		this->http->clientMaxBodySize = (1024 * 1024 * 1024);
}

void	HttpBuilder::handleErrorPage(const std::string &value)
{
	std::istringstream			iss(value);
	std::vector<std::string>	values;
	std::string					info;
	t_errorPage					*errorPage = new t_errorPage;

	this->http->errorPageDirective = true;

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
		this->http->errorPages[std::atol((*it).c_str())] = errorPage;
	}
}
