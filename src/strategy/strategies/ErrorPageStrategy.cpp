#include "../../../include/strategy/strategies/ErrorPageStrategy.hpp"

bool ErrorPageStrategy::validate(const std::string &value) const
{
    std::cout << "ErrorPage Strategy. Value: " << value << "\n";
    return (true);
}