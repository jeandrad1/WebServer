#include "LocationConfig.hpp"
#include <iostream>

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

LocationConfig::~LocationConfig(void)
{
    for (std::map<int, t_errorPage *>::iterator it = this->errorPages.begin(); it != this->errorPages.end(); ++it)
    {
        it->second->referencesCount--;
        if (it->second->referencesCount == 0)
            delete it->second;
    }
    delete this->_return;
}

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

void LocationConfig::printValues(int indent) 
{
    std::string	spaces(indent * 2, ' ');
    std::cout << spaces << BLUE "LocationConfig:\n" RESET;
    std::cout << spaces << YELLOW "  locationPath: " RESET << locationPath << "\n";
    std::cout << spaces << YELLOW "  root: " RESET << root << "\n";
    std::cout << spaces << YELLOW "  return:" RESET;
    if (_return)
    {
        std::cout << spaces << YELLOW "HTTP: " RESET << _return->http;
        std::cout << spaces << YELLOW "Code: " RESET << _return->code;
        std::cout << spaces << YELLOW "return directive: " RESET << (_return->returnDirective ? "true\n" : "false\n");
    }
    else
        std::cout << spaces << "  No return directive configured." << std::endl;

    std::cout << spaces << YELLOW "  error_page:\n" RESET;
    if (errorPages.empty())
        std::cout << spaces << "  No error_page configured." << std::endl;
    else
    {
        for (std::map<int, t_errorPage *>::const_iterator it = errorPages.begin(); it != errorPages.end(); ++it)
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
                std::cout << spaces << "  Null error page entry encountered for error code: " << it->first << std::endl;
        }
    }
    std::cout << spaces << YELLOW "  index: " RESET;
    for (size_t i = 0; i < index.size(); ++i)
        std::cout << index[i] << " ";
    std::cout << "\n";
    std::cout << spaces << YELLOW "  clientMaxBodySize: " RESET << clientMaxBodySize << "\n";
    if (limit_except) {
        std::cout << spaces << YELLOW "  limit_except: " RESET;
        std::cout << YELLOW"  POST: " RESET<< (limit_except->POST ? "true" : "false");
        std::cout << YELLOW"  GET: " RESET<< (limit_except->GET ? "true" : "false");
        std::cout << YELLOW"  DELETE: " RESET << (limit_except->DELETE ? "true" : "false");
        std::cout << YELLOW"  Limit Directive: " RESET<< (limit_except->limitDirective ? "true" : "false") << std::endl;
    } else {
        std::cout << spaces << "Limit Except: Not set" << std::endl;
    }

}
