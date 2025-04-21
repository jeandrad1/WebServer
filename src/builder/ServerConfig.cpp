#include "ServerConfig.hpp"

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

void ServerConfig::printValues(int indent) 
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
    std::cout << spaces << YELLOW "  return Directive:" RESET << std::endl;
    if (_return)
    {
        std::cout << spaces << "  HTTP: " << _return->http << std::endl;
        std::cout << spaces << "  Code: " << _return->code << std::endl;
        std::cout << spaces << "  Return Directive: " << (_return->returnDirective ? "true" : "false") << std::endl;
    }
    else
        std::cout << spaces << "  No return directive configured." << std::endl;

    std::cout << spaces << YELLOW "  Error_pages:" RESET << std::endl;
    if (errorPages.empty())
        std::cout << spaces << "  No error_page configured." << std::endl;
    else
    {
        for (std::map<int, t_errorPage *>::const_iterator it = errorPages.begin(); it != errorPages.end(); ++it)
        {
            if (it->second)
            {
                std::cout << spaces << "  Error Code: " << it->first << std::endl;
                std::cout << spaces << "  Target Page: " << it->second->targetPage << std::endl;
                std::cout << spaces << "  Is Equal Modifier: " << (it->second->isEqualModifier ? "true" : "false") << std::endl;
                std::cout << spaces << "  Equal Modifier: " << it->second->equalModifier << std::endl;
                std::cout << spaces << "  References Count: " << it->second->referencesCount << std::endl;
            }
            else
                std::cout << spaces << "  Null error page entry encountered for error code: " << it->first << std::endl;
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
    std::cout << "\n";
}
