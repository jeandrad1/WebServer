#include "ServerConfig.hpp"
#include "HttpConfig.hpp"

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

ServerConfig::~ServerConfig(void)
{
	for (std::map<int, t_errorPage *>::iterator it = this->_errorPages.begin(); it != this->_errorPages.end(); ++it)
	{
		it->second->referencesCount--;
		if (it->second->referencesCount == 0)
		{
			if (it->second)	
				delete it->second;
		}
	}
	if (this->_listen.size() != 0)
	{
		for (std::vector<t_listen *>::iterator it = this->_listen.begin(); it < this->_listen.end(); ++it)
		{
			delete (*it);
		}
	}
	delete this->_return;
	for (std::vector<LocationConfig *>::iterator it = this->_locations.begin(); it != this->_locations.end(); ++it)
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
	for (size_t i = 0; i < _listen.size(); ++i)
	{
		if (_listen[i])
			std::cout << spaces << YELLOW "IP: " RESET << _listen[i]->ip << ", " << YELLOW "Port: " RESET << _listen[i]->port << "\n";
		else
			std::cout << "  Null listen entry encountered.\n";
	}
	std::cout << spaces << YELLOW "  root: " RESET << _root << "\n";
	std::cout << spaces << YELLOW "  autoindex: " RESET << (_autoindex ? "on" : "off") << "\n";
	std::cout << spaces << YELLOW "  clientMaxBodySize: " RESET << _clientMaxBodySize << "\n";
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
	if (_errorPages.empty())
		std::cout << spaces << "  No error_page configured.\n";
	else
	{
		for (std::map<int, t_errorPage *>::const_iterator it = _errorPages.begin(); it != _errorPages.end(); ++it)
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
	for (size_t i = 0; i < _index.size(); ++i)
		std::cout << _index[i] << " ";
	std::cout << "\n";
	std::cout << spaces << YELLOW "  serverNames: " RESET;
	std::vector<std::string>::iterator ite = _serverNames.end();
	for (std::vector<std::string>::iterator it = _serverNames.begin(); it != ite; ++it)
		std::cout << (*it).c_str() << " ";
	std::cout << "\n\n";
	for (size_t i = 0; i < this->_locations.size(); i++)
		this->_locations[i]->LocationConfig::printValues(indent + 2);
}

void	ServerConfig::applyInheritedConfig(void)
{
	for (std::vector<LocationConfig *>::iterator it = _locations.begin(); it != _locations.end(); it++)
	{
		LocationConfig *location = *it;
		location->inheritFromServer(*this);
		location->applyInheritedConfig();
	}
	this->defaultInheritedValues();
}

void	ServerConfig::inheritFromHttp(const HttpConfig &http)
{
	if (this->_clientMaxBodySize == -1 && http.clientMaxBodySize != -1)
		this->_clientMaxBodySize = http.clientMaxBodySize;
	if (http.errorPageDirective && !_errorPageDirective) 
	{
		_errorPageDirective = true;
		_errorPages.clear();
		for (std::map<int, t_errorPage *>::const_iterator it = http.errorPages.begin(); it != http.errorPages.end(); ++it)
		{
			t_errorPage *orig = it->second;
			if (orig) {
				t_errorPage *copy = new t_errorPage;
				*copy = *orig;
				copy->referencesCount = 1;
				_errorPages[it->first] = copy;
			}
			else {
				_errorPages[it->first] = NULL;
			}
		}
	}
}

void	ServerConfig::defaultInheritedValues(void)
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

void	ServerConfig::setLocations(std::vector<LocationConfig*>& src)
{_locations = src;}

void	ServerConfig::setListen(std::vector<t_listen *> &src)
{_listen = src;}

void	ServerConfig::setListenDirective(bool src)
{_listenDirective = src;}

void	ServerConfig::setReturn(t_return *src)
{_return = src;}

void	ServerConfig::setReturnHttp(std::string &http)
{_return->http = http;}

void	ServerConfig::setReturnCode(int src)
{_return->code = src;}

void	ServerConfig::setReturnDirective(bool src)
{_return->returnDirective = src;}

void	ServerConfig::setClientMaxBodySize(long long src)
{_clientMaxBodySize = src;}

void	ServerConfig::setAutoIndex(bool src)
{_listenDirective = src;}

void	ServerConfig::setRoot(std::string &src)
{_root = src;}

void	ServerConfig::setErrorPages(std::map<int, t_errorPage *> &src)
{_errorPages = src;}

void	ServerConfig::setErrorPageDirective(bool src)
{_errorPageDirective = src;}

void	ServerConfig::setServerNames(std::vector<std::string> &src)
{_serverNames = src;}

void	ServerConfig::setIndex(std::vector<std::string> &src)
{_index = src;}

/***********************************************************************/
/*                          Adders                                    */
/***********************************************************************/

void	ServerConfig::addLocation(LocationConfig* location)
{_locations.push_back(location);}

void	ServerConfig::addListen(t_listen *listen)
{_listen.push_back(listen);}

void	ServerConfig::addErrorPage(int src1, t_errorPage *src2)
{_errorPages[src1] = src2;}

void	ServerConfig::addServerNames(std::string serverName)
{_serverNames.push_back(serverName);}

void	ServerConfig::addIndex(std::string Index)
{_index.push_back(Index);}

/***********************************************************************/
/*                          Getters                                    */
/***********************************************************************/

const std::vector<LocationConfig*> &ServerConfig::getLocations() const
{return _locations;}

const std::vector<t_listen *>	 &ServerConfig::getListen() const
{return _listen;}

bool ServerConfig::getListenDirective() const
{return _listenDirective;}

t_return *ServerConfig::getReturn() const
{return _return;}

long long ServerConfig::getClientMaxBodySize() const
{return _clientMaxBodySize;}

bool ServerConfig::getAutoIndex() const
{return _listenDirective;}

std::string ServerConfig::getRoot() const
{return _root;}

const std::map<int, t_errorPage *> ServerConfig::getErrorPages() const
{return _errorPages;}

bool ServerConfig::getErrorPageDirective() const
{return _errorPageDirective;}

std::vector<std::string> ServerConfig::getServerNames() const
{return _serverNames;}

std::vector<std::string> ServerConfig::getIndex() const
{return _index;}

/***********************************************************************/
/*                          Clearers                                   */
/***********************************************************************/

void ServerConfig::clearListen()
{
	for (size_t i = 0; i < _listen.size(); ++i)
		delete _listen[i];
	_listen.clear();
}

void ServerConfig::clearServerName()
{
	_serverNames.clear();
}