#include "../../../include/strategy/strategies/ServerNameStrategy.hpp"
#include <cctype>

bool ServerNameStrategy::validate(const std::string &value) const
{
    if (value.empty())
        return false;
    
    if (value.length() < 1 || value.length() > 253)
        return false;

    for (size_t i = 0; i < value.length(); ++i)
    {
        char c = value[i];
        if (!std::isalnum(c) && c != '.' && c != '-')
            return false;
    
        if (c == '.' && i > 0 && value[i - 1] == '.')
            return false;
    }

    if (value.front() == '-' || value.back() == '-')
        return false;

    return true;
}
