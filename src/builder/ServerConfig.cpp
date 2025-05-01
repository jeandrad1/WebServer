#include "ServerConfig.hpp"
#include "HttpConfig.hpp"

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

ServerConfig::~ServerConfig(void)
{
	for (std::map<int, t_errorPage *>::iterator it = this->errorPages.begin(); it != this->errorPages.end(); ++it)
	{
		it->second->referencesCount--;
		if (it->second->referencesCount == 0)
			delete it->second;
	}
	if (this->listen.size() != 0)
	{
		for (std::vector<t_listen *>::iterator it = this->listen.begin(); it < this->listen.end(); ++it)
		{
			delete (*it);
		}
	}
	delete this->_return;
	for (std::vector<LocationConfig *>::iterator it = this->locations.begin(); it != this->locations.end(); ++it)
		delete (*it);
}

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

void	ServerConfig::printValues(int indent) 
{
	std::string	spaces(indent * 2, ' ');
	std::cout << spaces << BLUE "ServerConfig:\n" RESET;
	std::cout << spaces << YELLOW "  listen: " RESET;
	for (size_t i = 0; i < listen.size(); ++i)
	{
		if (listen[i])
			std::cout << spaces << YELLOW "IP: " RESET << listen[i]->ip << ", " << YELLOW "Port: " RESET << listen[i]->port << "\n";
		else
			std::cout << "  Null listen entry encountered.\n";
	}
	std::cout << spaces << YELLOW "  root: " RESET << root << "\n";
	std::cout << spaces << YELLOW "  autoindex: " RESET << (autoindex ? "on" : "off") << "\n";
	std::cout << spaces << YELLOW "  clientMaxBodySize: " RESET << clientMaxBodySize << "\n";
	std::cout << spaces << YELLOW "  return:" RESET;
	if (_return)
	{
		std::cout << spaces << YELLOW " HTTP: " RESET << _return->http;
		std::cout << spaces << YELLOW " Code: " RESET << _return->code;
		std::cout << spaces << YELLOW " return directive: " RESET << (_return->returnDirective ? "true\n" : "false\n");
	}
	else
		std::cout << spaces << "  No return directive configured.\n";

	std::cout << spaces << YELLOW "  error_pages:" RESET;
	if (errorPages.empty())
		std::cout << spaces << "  No error_page configured.\n";
	else
	{
		for (std::map<int, t_errorPage *>::const_iterator it = errorPages.begin(); it != errorPages.end(); ++it)
		{
			if (it->second)
			{
				std::cout << spaces << YELLOW "    Error Code: " RESET << it->first;
				std::cout << spaces << YELLOW "Target Page: " RESET << it->second->targetPage;
				std::cout << spaces << YELLOW "Is Equal Modifier: " RESET << (it->second->isEqualModifier ? "true" : "false");
				std::cout << spaces << YELLOW "Equal Modifier: " RESET << it->second->equalModifier << "\n";
			}
			else
				std::cout << spaces << "  Null error page entry encountered for error code: " << it->first << "\n";
		}
	}
	std::cout << spaces << YELLOW "  index: " RESET;
	for (size_t i = 0; i < index.size(); ++i)
		std::cout << index[i] << " ";
	std::cout << "\n";
	std::cout << spaces << YELLOW "  serverNames: " RESET;
	std::vector<std::string>::iterator ite = serverNames.end();
	for (std::vector<std::string>::iterator it = serverNames.begin(); it != ite; ++it)
		std::cout << (*it).c_str() << " ";
	std::cout << "\n\n";
	for (size_t i = 0; i < this->locations.size(); i++)
		this->locations[i]->LocationConfig::printValues(indent + 2);
}


void	ServerConfig::inherance(void)
{
	std::vector<LocationConfig *>::iterator it = locations.begin();
	for (; it != locations.end(); it++)
	{
		LocationConfig *location = *it;
		location->inheritFromServer(*this);
		location->inherance();
	}
	this->defaultInheritValues();
}

void	ServerConfig::inheritFromHttp(const HttpConfig &http)
{
	if (this->clientMaxBodySize == -1 && http.clientMaxBodySize != -1)
		this->clientMaxBodySize = http.clientMaxBodySize;
	if (http.errorPageDirective && !errorPageDirective) 
	{
		errorPageDirective = true;
		errorPages = http.errorPages;
	}
}

void	ServerConfig::defaultInheritValues(void)
{
	if (clientMaxBodySize == -1)
		clientMaxBodySize = DEFAULT_CLIENT_MAX_BODY_SIZE;
	if(this->root == "-1")
		this->root = DEFAULT_ROOT;
	if (this->index[0] == " ")
	{
		this->index.clear();
		this->index.push_back("index.html");
		this->index.push_back("index.htm");
	}
}