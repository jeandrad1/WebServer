#include <string>

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