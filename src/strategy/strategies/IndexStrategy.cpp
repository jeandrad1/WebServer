#include "../../../include/strategy/strategies/IndexStrategy.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <cctype>

bool check_chars(const std::string &value)
{
    for (size_t i = 0; i < value.size(); i++)
    {
        char c = value[i];
        if (c > 31 && c < 127)
            return true; // Only needs one valid character
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

int count_whitespace(const std::string &str)
{
    int count = 0;
    for (size_t i = 0; i < str.size(); i++)
    {
        if (std::isspace(str[i]))
            count++;
    }
    return count;
}

bool check_filename(const std::string &word)
{
    return word.size() >= 5 && word.substr(word.size() - 5) == ".html";
}

bool split(const std::string &value)
{
    std::istringstream iss(value);
    std::string word;
    while (iss >> word)
    {
        if (!check_filename(word))
            return false;
    }
    return true;
}

bool IndexStrategy::validate(const std::string &value) const
{
    if (value.empty())
        return false;

    if (!check_chars(value))
        return false;

    if (count_whitespace(value) == (int)value.length())
        return false;

    std::string real_value = trim(value);
    if (real_value.empty())
        return false;

    return split(real_value);
}
