#include "../../../include/strategy/strategies/ServerNameStrategy.hpp"
#include <cctype>
#include <string>

std::string getSubstringBeforeSemicolon(const std::string &value);

bool ServerNameStrategy::validate(const std::string &value) const
{
    if (value.size() == 0)
        return false;

    if (value[value.size() - 1] != ';')
        return false;

    std::string real_value = getSubstringBeforeSemicolon(value);

    if (real_value.size() < 1 || real_value.size() > 253)
        return false;

    for (size_t i = 0; i < real_value.size(); ++i)
    {
        char c = real_value[i];
        if (!std::isalnum(static_cast<unsigned char>(c)) && c != '.' && c != '-')
            return false;

        if (c == '.' && i > 0 && real_value[i - 1] == '.')
            return false;
    }

    if (real_value[0] == '-' || real_value[value.size() - 1] == '-')
        return false;

    return true;
}
