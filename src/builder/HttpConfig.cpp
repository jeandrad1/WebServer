#include "HttpConfig.hpp"

void HttpConfig::printValues() const
{
    std::cout << "HttpConfig:\n";
    std::cout << "  clientMaxBodySize: " << clientMaxBodySize << "\n";
    std::cout << "  errorPages: " << errorPages.size() << "\n";
}
