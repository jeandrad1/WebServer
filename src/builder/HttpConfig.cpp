#include "HttpConfig.hpp"

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

HttpConfig::~HttpConfig()
{
    for (std::map<int, t_errorPage *>::iterator it = this->errorPages.begin(); it != this->errorPages.end(); ++it)
    {
        it->second->referencesCount--;
        if (it->second->referencesCount == 0)
            delete it->second;
    }
    for (std::vector<ServerConfig *>::iterator it = this->servers.begin(); it != this->servers.end(); ++it)
        delete (*it);
}

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

void HttpConfig::printValues(int indent) 
{
    std::string	spaces(indent * 2, ' ');
    std::cout << spaces << BLUE "HttpConfig:\n" RESET;
    std::cout << spaces << YELLOW "  clientMaxBodySize: " RESET << clientMaxBodySize << "\n";
    std::cout << spaces << YELLOW "  errorPages: " RESET << errorPages.size() << "\n\n";
    for (size_t i = 0; i < this->servers.size(); i++)
    this->servers[i]->ServerConfig::printValues(indent + 2);
    std::cout << "\n";
}
