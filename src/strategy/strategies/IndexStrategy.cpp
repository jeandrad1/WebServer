#include "include/strategy/IndexStrategy.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <cctype>

bool check_chars(std::string value)
{
    long count = 0;
    for (char c : value)
    {
        if (c > 31 && c < 127)
            count++;
    }
    return count != 0;
}

std::string trim(const std::string &str)
{
    if (str.empty())
        return "";
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos)
        return "";

    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}


int count_whitespace(const std::string &str)
{
    int i = 0;
    int j = 0;
    while (i < str.length())
    {
        if (std::isspace(str[i]) == 0)
            i++;
        j++;
    }
    return (j);
}

bool correct_end(const std::string &value)
{
    if (value.find(' ') == std::string::npos)
    {
        if (value.size() > 5 && value.substr(value.size() - 5) == ".html")
            return true;
        else
            return false;
    }
    return false;
}

bool split(const std::string &value)
{
    std::istringstream iss(value);
    std::string word;
    while (iss >> word)
    {
        if (word.size() > 5 && word.substr(word.size() - 5) == ".html")
            continue;
        else
            return false;
    }
    return true;
}

bool IndexStrategy::validate(std::string const &value) const
{
    // check if null
    if (value.empty())
        return false;
    
    // check there are some spaces and letters
    if (check_chars(value) == false)
        return false;
    
    // Check to see that it is not just empty spaces
    if (count_whitespace(value) == value.length())
        return false;

    // Trim the ends
    std::string real_value = trim(value);
    if (real_value.empty())
		return false;

    // If one word do a .html end check
    if (correct_end(real_value) == false)
		return false;

    // Else split and do a loop of the function before
    if (split(real_value) == false)
		return false;

    return true;
}