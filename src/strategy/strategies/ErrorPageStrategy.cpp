#include "../../../include/strategy/strategies/ErrorPageStrategy.hpp"
#include <iostream>
#include <sstream>
#include <stdlib.h>

bool isInteger(std::string value);
bool isURI(const std::string &path);
bool isURL(const std::string &path);
bool checkValidErrorCode(int code);
bool checkCodeIsValid(int code);
bool validateEqualModifier(std::string str);
bool checkIfStringIsValid(std::string str, std::istringstream &iss, int &possibleStringValues);

/*  error_page [error_code ...] [=[response_code]] uri; */

bool ErrorPageStrategy::validate(const std::string &value) const
{
    std::istringstream iss(value);
    int possibleStringValues = 0;
    
    while (!iss.eof())
    {
        std::string string;
        
        iss >> string;
        if (isInteger(string))
        {
            if (possibleStringValues > 0)
                return (false);
            if (!checkValidErrorCode(atoi(string.c_str())))
                return (false);
        }
        else
        {
            if (possibleStringValues >= 2)
                return (false);
            if (!checkIfStringIsValid(string, iss, possibleStringValues))
            {
                return (false);
            }
            possibleStringValues++;
        }
    }
    return true;
}

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

bool isURI(const std::string &path)
{
    if (path[0] == '/')
        return (true);
    return (false);
}

bool isURL(const std::string &path)
{
    if (path.find("http://") == 0 || path.find("https://") == 0)
        return (true);
    return (false);
}

bool checkValidErrorCode(int code)
{
    if (code >= 400 && code <= 599)
        return (true);
    return (false);
}

bool checkCodeIsValid(int code)
{
    if (code >= 100 && code <= 599)
        return (true);
    return (false);
}

bool validateEqualModifier(std::string str)
{
    size_t equalPosition = str.find('=');
    if (equalPosition == std::string::npos)
        return (true);
    
    std::string code = str.substr(equalPosition + 1);
    if (checkCodeIsValid(atoi(code.c_str())))
        return (true);
    return (false);
}

bool checkIfStringIsValid(std::string str, std::istringstream &iss, int &possibleStringValues)
{
    if (validateEqualModifier(str) && !iss.eof() && possibleStringValues == 0)
        return (true);
    
    if (((isURI(str) && !isURL(str)) || (!isURI(str) && isURL(str))))
    {
        if (possibleStringValues == 0 || possibleStringValues == 1)
            possibleStringValues = 2; 
        if (str.find(';') != std::string::npos)
        {
            std::cout << "String: " << str << "\n";
            return (true);
        }
        else
            return (false);
    }
    return (false);
}