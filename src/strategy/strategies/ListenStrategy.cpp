#include "../../../include/strategy/strategies/ListenStrategy.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cctype>

static std::string get_substring_before_semicolon(const std::string &value)
{
    if (value.empty() || value[value.size() - 1] != ';')
        return "";

    return value.substr(0, value.size() - 1);
}

bool is_valid_port(const std::string &port_str)
{
    if (port_str.empty() || port_str.size() > 5)
        return false;

    for (size_t i = 0; i < port_str.size(); ++i)
    {
        if (!std::isdigit(static_cast<unsigned char>(port_str[i])))
            return false;
    }

    int port = std::atoi(port_str.c_str());
    return port >= 1 && port <= 65535;
}

bool is_valid_ip(const std::string &ip_str)
{
    std::istringstream iss(ip_str);
    std::string segment;
    int count = 0;

    while (std::getline(iss, segment, '.'))
    {
        if (segment.empty() || segment.size() > 3)
            return false;

        for (size_t i = 0; i < segment.size(); ++i)
        {
            if (!std::isdigit(static_cast<unsigned char>(segment[i])))
                return false;
        }

        int num = std::atoi(segment.c_str());
        if (num < 0 || num > 255)
            return false;

        count++;
    }
    return count == 4;
}

bool ListenStrategy::validate(const std::string &value) const
{
    if (value.empty())
        return false;

    if (value[value.size() - 1] != ';')
        return false;

    std::string real_value = get_substring_before_semicolon(real_value);

    size_t colon_pos = real_value.find(':');

    if (colon_pos != std::string::npos)
    {
        std::string ip = real_value.substr(0, colon_pos);
        std::string port = real_value.substr(colon_pos + 1);

        return is_valid_ip(ip) && is_valid_port(port);
    }
    return is_valid_port(real_value);
}
