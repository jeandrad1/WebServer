#include "HttpConfig.hpp"

void HttpConfig::printValues()
{
    std::cout << "HttpConfig:\n";
    std::cout << "  clientMaxBodySize: " << clientMaxBodySize << "\n";
    std::cout << "  errorPages: " << errorPages.size() << "\n";
}
