#include "LocationConfig.hpp"
#include "ServerConfig.hpp"

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

LocationConfig::~LocationConfig(void)
{
	for (std::map<int, t_errorPage *>::iterator it = this->_errorPages.begin(); it != this->_errorPages.end(); ++it)
	{
		it->second->referencesCount--;
		if (it->second->referencesCount == 0)
			delete it->second;
	}
	if (this->cgi.size() != 0)
	{
		for (std::vector<t_cgi *>::iterator it = this->cgi.begin(); it < this->cgi.end(); ++it)
		{
			delete (*it);
		}
	}
	delete this->_return;
}

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

void	LocationConfig::printValues(int indent) 
{
	std::string	spaces(indent * 2, ' ');
	std::cout << spaces << BLUE "LocationConfig:\n" RESET;
	std::cout << spaces << YELLOW "  locationPath: " RESET << _locationPath << "\n";
	std::cout << spaces << YELLOW "  root: " RESET << _root << "\n";
	std::cout << spaces << YELLOW "  return:" RESET;
	if (_return)
	{
		std::cout << spaces << YELLOW "HTTP: " RESET << _return->http;
		std::cout << spaces << YELLOW "Code: " RESET << _return->code;
		std::cout << spaces << YELLOW "return directive: " RESET << (_return->returnDirective ? "true\n" : "false\n");
	}
	else
		std::cout << spaces << "  No return directive configured.\n";

	std::cout << spaces << YELLOW "  error_page:\n" RESET;
	if (_errorPages.empty())
		std::cout << spaces << "  No error_page configured.\n";
	else
	{
		for (std::map<int, t_errorPage *>::const_iterator	it = _errorPages.begin(); it != _errorPages.end(); ++it)
		{
			if (it->second)
			{
				std::cout << spaces << YELLOW "    Error Code: " RESET << it->first;
				std::cout << spaces << YELLOW "Target Page: " RESET << it->second->targetPage;
				std::cout << spaces << YELLOW "Is Equal Modifier: " RESET << (it->second->isEqualModifier ? "true" : "false");
				std::cout << spaces << YELLOW "Equal Modifier: " RESET << it->second->equalModifier;
				std::cout << "\n";
			}
			else
				std::cout << spaces << "  Null error page entry encountered for error code: " << it->first << "\n";
		}
	}

	std::cout << spaces << YELLOW "  index: " RESET;
	for (size_t i = 0; i < _index.size(); ++i)
		std::cout << _index[i] << " ";
	std::cout << "\n";

	std::cout << spaces << YELLOW "  clientMaxBodySize: " RESET << _clientMaxBodySize << "\n";
	if (_limit_except)
	{
		std::cout << spaces << YELLOW "  limit_except: " RESET;
		std::cout << YELLOW"  POST: " RESET<< (_limit_except->POST ? "true" : "false");
		std::cout << YELLOW"  GET: " RESET<< (_limit_except->GET ? "true" : "false");
		std::cout << YELLOW"  DELETE: " RESET << (_limit_except->DELETE ? "true" : "false");
		std::cout << YELLOW"  Limit Directive: " RESET<< (_limit_except->limitDirective ? "true\n" : "false\n");
	}
	else
		std::cout << spaces << "Limit Except: Not set\n";
}

void	LocationConfig::applyInheritedConfig(void)
{
	this->defaultInheritedValues();
}

void	LocationConfig::inheritFromServer(const ServerConfig &server)
{
	if (_clientMaxBodySize == -1 && server.clientMaxBodySize != -1)
		_clientMaxBodySize = server.clientMaxBodySize;
	if (server.errorPageDirective && !_errorPageDirective)
	{
		_errorPageDirective = true;
		_errorPages = server.errorPages;
	}
	if (_root == "-1" && server.root != "-1")
		_root = server.root;
	if (_index[0] == " " && server.index[0] != " ")
	{
		_index.clear();
		_index = server.index;
	}
}

void	LocationConfig::defaultInheritedValues(void)
{
	if (_clientMaxBodySize == -1)
		_clientMaxBodySize = DEFAULT_CLIENT_MAX_BODY_SIZE;
	if(this->_root == "-1")
		this->_root = DEFAULT_ROOT;
	if (this->_index[0] == " ")
	{
		this->_index.clear();
		this->_index.push_back("index.html");
		this->_index.push_back("index.htm");
	}
}

/***********************************************************************/
/*                          Setters                                    */
/***********************************************************************/

void LocationConfig::setLocationPath(std::string &src)
{_locationPath = src;}

void LocationConfig::setRoot(std::string &src)
{_root = src;}

void LocationConfig::setIndex(std::vector<std::string> &src)
{_index = src;}

void LocationConfig::setAutoIndex(bool src)
{_autoindex = src;}

void LocationConfig::setClientMaxBodySize(long long src)
{_clientMaxBodySize = src;}

void LocationConfig::setErrorPages(std::map<int, t_errorPage *> &src)
{_errorPages = src;}

void LocationConfig::setErrorPageDirective(bool src)
{_errorPageDirective = src;}

void LocationConfig::setLimitExcept(t_limit_except *src)
{_limit_except = src;}

void LocationConfig::setReturn(t_return *src)
{_return = src;}

void LocationConfig::setReturnHttp(std::string &http)
{_return->http = http;}

void LocationConfig::setReturnCode(int src)
{_return->code = src;}

void LocationConfig::setReturnDirective(bool src)
{_return->returnDirective = src;}

void LocationConfig::setLimitExceptLimitDirective(bool src)
{_limit_except->limitDirective = src;}

void LocationConfig::setLimitExceptPost(bool src)
{_limit_except->POST = src;}

void LocationConfig::setLimitExceptGet(bool src)
{_limit_except->GET = src;}

void LocationConfig::setLimitExceptDelete(bool src)
{_limit_except->DELETE = src;}

/***********************************************************************/
/*                          Getters                                    */
/***********************************************************************/

std::string LocationConfig::getLocationPath() const
{return _locationPath;}

std::string LocationConfig::getRoot() const
{return _root;}

const std::vector<std::string> LocationConfig::getIndex() const
{return _index;}

bool LocationConfig::getAutoIndex() const
{return _autoindex;}

long long LocationConfig::getClientMaxBodySize() const
{return _clientMaxBodySize;}

t_return *LocationConfig::getReturn() const
{return _return;}

const std::map<int, t_errorPage *> LocationConfig::getErrorPages() const
{return _errorPages;}

bool LocationConfig::getErrorPageDirective() const
{return _errorPageDirective;}

t_limit_except *LocationConfig::getLimitExcept() const
{return _limit_except;}