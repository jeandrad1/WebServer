#include "ServerConfig.hpp"

void ServerConfig::printValues() const 
{
    std::cout << "ServerConfig:\n";
    std::cout << "  root: " << root << "\n";
    std::cout << "  autoindex: " << (autoindex ? "on" : "off") << "\n";
    std::cout << "  serverNames: ";
    for (size_t i = 0; i < serverNames.size(); ++i)
        std::cout << serverNames[i] << " ";
    std::cout << "\n";
}
