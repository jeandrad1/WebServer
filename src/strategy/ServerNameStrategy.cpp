#include "ServerNameStrategy.hpp"
#include <sstream>

std::string	getSubstringBeforeSemicolon(const std::string &value);
std::string	trim(const std::string &str);

bool	ServerNameStrategy::validate(const std::string &value) const
{
    if (value.empty())
        return (false);

    if (value[value.size() - 1] != ';')
        return (false);

    std::string real_value = getSubstringBeforeSemicolon(value);

    if (real_value.empty())
        return (false);

    std::istringstream	iss(real_value);
    std::string 		server_name;

    while (iss >> server_name)
    {
        server_name = trim(server_name);

        if (server_name.size() < 1 || server_name.size() > 253)
            return (false);

        for (size_t i = 0; i < server_name.size(); ++i)
        {
            char c = server_name[i];
            if (!std::isalnum(static_cast<unsigned char>(c)) && c != '.' && c != '-')
                return (false);

            if (c == '.' && i > 0 && server_name[i - 1] == '.')
                return (false);
        }

        if (server_name[0] == '-' || server_name[server_name.size() - 1] == '-')
            return (false);
    }

    return (true);
}