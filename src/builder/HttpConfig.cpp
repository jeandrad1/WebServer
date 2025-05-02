#include "HttpConfig.hpp"
#include "ServerConfig.hpp"

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

HttpConfig::~HttpConfig()
{
	for (std::map<int, t_errorPage *>::iterator	it = this->errorPages.begin(); it != this->errorPages.end(); ++it)
	{
		it->second->referencesCount--;
		if (it->second->referencesCount == 0)
			delete it->second;
	}
	for (std::vector<ServerConfig *>::iterator	it = this->servers.begin(); it != this->servers.end(); ++it)
		delete (*it);
}

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

void	HttpConfig::printValues(int indent) 
{
	std::string	spaces(indent * 2, ' ');
	std::cout << spaces << BLUE "HttpConfig:\n" RESET;
	std::cout << spaces << YELLOW "  clientMaxBodySize: " RESET << clientMaxBodySize << "\n";
	std::cout << spaces << YELLOW "  error_page:\n" RESET;
	if (errorPages.empty())
		std::cout << spaces << "  No error_page configured.\n";
	else
	{
		for (std::map<int, t_errorPage *>::const_iterator	it = errorPages.begin(); it != errorPages.end(); ++it)
		{
			if (it->second)
			{
				std::cout << spaces << YELLOW "    Error Code: " RESET << it->first;
				std::cout << spaces << YELLOW " Target Page: " RESET << it->second->targetPage;
				std::cout << spaces << YELLOW " Is Equal Modifier: " RESET << (it->second->isEqualModifier ? "true" : "false");
				std::cout << spaces << YELLOW " Equal Modifier: " RESET << it->second->equalModifier << "\n";
			}
			else
				std::cout << spaces << "  Null error page entry encountered for error code: " << it->first << "\n";
		}
	}
	std::cout << "\n";
	for (size_t	i = 0; i < this->servers.size(); i++)
		this->servers[i]->ServerConfig::printValues(indent + 2);
	std::cout << "\n";
}

void	HttpConfig::applyInheritedConfig(void)
{
	for(std::vector<ServerConfig *>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		ServerConfig *server = *it;
		server->inheritFromHttp(*this);
		server->applyInheritedConfig();
	}
	this->defaultInheritedValues();
}

void	HttpConfig::defaultInheritedValues(void)
{
	if (clientMaxBodySize == -1)
		clientMaxBodySize = DEFAULT_CLIENT_MAX_BODY_SIZE;
}
