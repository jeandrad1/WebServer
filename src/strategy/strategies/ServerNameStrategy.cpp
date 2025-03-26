#include "../../../include/strategy/strategies/ServerNameStrategy.hpp"
#include <cctype>
#include <string>

bool validate(const std::string &value)
{
    if (value.size() == 0)
        return false;

    if (value.size() < 1 || value.size() > 253)
        return false;

    for (size_t i = 0; i < value.size(); ++i)
    {
        char c = value[i];
        if (!std::isalnum(static_cast<unsigned char>(c)) && c != '.' && c != '-')
            return false;

        if (c == '.' && i > 0 && value[i - 1] == '.')
            return false;
    }

    if (value[0] == '-' || value[value.size() - 1] == '-')
        return false;

    return true;
}
