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
    std::cout << spaces << YELLOW "  index: " RESET;
    for (size_t i = 0; i < index.size(); ++i)
        std::cout << index[i] << " ";
    std::cout << "\n";
    std::cout << spaces << YELLOW "  clientMaxBodySize: " RESET << clientMaxBodySize << "\n";
}
