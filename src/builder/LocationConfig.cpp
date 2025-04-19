#include "LocationConfig.hpp"
#include <iostream>

void LocationConfig::printValues()
{
    std::cout << "LocationConfig:\n";
    std::cout << "  locationPath: " << locationPath << "\n";
    std::cout << "  root: " << root << "\n";
    std::cout << "  index: ";
    // for (size_t i = 0; i < index.size(); ++i)
    //     std::cout << index[i] << " ";
    std::cout << "\n";
}
