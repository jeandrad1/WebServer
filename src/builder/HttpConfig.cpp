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
    std::cout << spaces << YELLOW "  Error_page:" RESET << std::endl;
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
    for (size_t i = 0; i < this->servers.size(); i++)
    this->servers[i]->ServerConfig::printValues(indent + 2);
    std::cout << "\n";
}
