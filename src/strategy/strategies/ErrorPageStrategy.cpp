#include "../../../include/strategy/strategies/ErrorPageStrategy.hpp"
#include <iostream>
#include <sstream>
#include <stdlib.h>

/*  error_page [error_code ...] [=[response_code]] uri; */

bool isInteger(std::string value)
{
    if (value.empty())
        return (false);

    for (size_t i = 0; i < value.size(); ++i)
    {
        if (!isdigit(value[i]))
            return (false);
    }
    return (true);
}

bool checkIfCodeIsValid(int code)
{
    if (code >= 400 && code <= 599)
        return (true);
    return (false);
}

bool ErrorPageStrategy::validate(const std::string &value) const
{
    std::cout << "ErrorPage Strategy. Value: " << value << "\n";

    std::istringstream iss(value);

    while (!iss.eof())
    {
        std::string string;

        iss >> string;
        if (isInteger(string))
        {
            std::cout << "Is integer: " << string << "\n";
            if (checkIfCodeIsValid(atoi(string.c_str())))
                std::cout << "Error code is valid\n";
            else
                std::cout << "Error code is invalid\n";
        }
        else
        {
            std::cout << "Is string: " << string << "\n";
            if (string[0] == '=' && iss.eof())
                return (false);
        }
    }
    return true;
}