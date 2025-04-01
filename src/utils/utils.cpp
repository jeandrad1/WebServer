#include <iostream>

std::string getSubstringBeforeSemicolon(const std::string &value)
{
    if (value.empty() || value[value.size() - 1] != ';')
        return "";

    return value.substr(0, value.size() - 1);
}

bool checkChars(const std::string &value)
{
    for (size_t i = 0; i < value.size(); i++)
    {
        char c = value[i];
        if (c > 31 && c < 127)
            return true;
    }
    return false;
}

std::string trim(const std::string &str)
{
    if (str.size() == 0)
        return "";

    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos)
        return "";

    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

int countWhitespace(const std::string &str)
{
    int count = 0;
    for (size_t i = 0; i < str.size(); i++)
    {
        if (std::isspace(str[i]))
            count++;
    }
    return count;
}

bool checkFilename(const std::string &word)
{
    return word.size() >= 5 && word.substr(word.size() - 5) == ".html";
}
