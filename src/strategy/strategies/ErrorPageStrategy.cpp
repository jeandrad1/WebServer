#include "../../../include/strategy/strategies/ErrorPageStrategy.hpp"
#include <iostream>
#include <sstream>

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

/*bool checkIfCodeIsValid(int code)
{
    if (code)
}*/

bool ErrorPageStrategy::validate(const std::string &value) const
{
    std::cout << "ErrorPage Strategy. Value: " << value << "\n";

    std::istringstream iss(value);

    while (!iss.eof())
    {
        std::string val;

        iss >> val;
        if (isInteger(val))
        {
            int val;
            iss >> val;
            std::cout << "Is integer: " << val << "\n";
        }
        else
        {
            std::string page;
            iss >> page;
            std::cout << "Is string: " << page << "\n";
        }
    }
    return true;
}