#include "ServerConfig.hpp"

void ServerConfig::printValues() 
{
    if(this)
        std::cout << "existe\n";
    std::cout << "ServerConfig:\n";
    std::cout << "  root: " << root << "\n";
    std::cout << "  autoindex: " << (autoindex ? "on" : "off") << "\n";
    std::cout << "  serverNames: ";
    //std::cout << this->serverNames[0] << "\n";
    //std::vector<std::string>::iterator ite = serverNames.end();
    //for (std::vector<std::string>::iterator it = serverNames.begin(); it != ite; ++it)
    //    std::cout << (*it).c_str() << " ";
    for (size_t i = 0; i < this->locations.size(); i++)
        this->locations[i]->LocationConfig::printValues();
    std::cout << "\n";
}
