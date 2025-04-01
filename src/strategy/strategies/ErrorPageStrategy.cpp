#include "../../../include/strategy/strategies/ErrorPageStrategy.hpp"
#include <iostream>
#include <sstream>
#include <stdlib.h>

bool isInteger(std::string value);
bool isURI(const std::string &path);
bool isURL(const std::string &path);
bool checkValidErrorCode(int code);
bool checkCodeIsValid(int code);
static bool validateEqualModifier(std::string str);
static bool checkIfStringIsValid(std::string str, std::istringstream &iss, int &possibleStringValues);

/* Syntax for error_page: 
    
    error_page [error_code ...] [=[response_code]] uri;

*/

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
            if (!checkIfStringIsValid(string, iss, possibleStringValues))
            {
                return (false);
            }
            possibleStringValues++;
        }
    }
    return (true);
}

static bool validateEqualModifier(std::string str)
{
    size_t equalPosition = str.find('=');
    if (equalPosition == std::string::npos && !isURI(str) && !isURL(str) && isInteger(str))
        return (true);

    std::string code = str.substr(equalPosition + 1);
    if (checkCodeIsValid(atoi(code.c_str())))
        return (true);
    return (false);
}

static bool checkIfStringIsValid(std::string str, std::istringstream &iss, int &possibleStringValues)
{
    if (validateEqualModifier(str) && !iss.eof() && possibleStringValues == 0)
        return (true);

    if (((isURI(str) && !isURL(str)) || (!isURI(str) && isURL(str))))
    {
        if (str.find(';') != std::string::npos)
            return (true);
        else
            return (false);
    }
    return (false);
}