#include "ReturnStrategy.hpp"
#include <vector>

static bool isCode(std::string const &value)
{
    std::vector<std::string> valid;
    valid.push_back("200"); valid.push_back("204"); valid.push_back("206");
    valid.push_back("301"); valid.push_back("302"); valid.push_back("303"); valid.push_back("307"); valid.push_back("308");
    valid.push_back("400"); valid.push_back("401"); valid.push_back("403"); valid.push_back("404"); valid.push_back("405"); valid.push_back("410");
    valid.push_back("500"); valid.push_back("502"); valid.push_back("503"); valid.push_back("504");
    if(value.size() != 3)
        return false;
    for(unsigned long i = 0; i < value.size(); i++)
    {
        if (!std::isdigit(value[i]))
            return false;
    }
    std::vector<std::string>::iterator it = valid.begin();
    for (std::vector<std::string>::iterator ite = valid.end(); it != ite; it++)
    {
        if (value == *it)
            return true;
    }
    return false;
}

static bool oneParameter(std::string const &arg)
{
    if (isCode(arg) == true && (arg[0] == '4' || arg[0] == '5' || arg == "200" || arg == "204"))
        return true;
    if (arg.substr(0, 8) != "https://" && arg.substr(0, 7) != "http://")
        return false;
    return true;
}

static bool twoParameter(std::string const &code, std::string const &direction)
{
	if (isCode(code) == false)
		return false;
	if (direction.substr(0, 8) != "https://" && direction.substr(0, 7) != "http://")
        return false;
	return true;
}

bool ReturnStrategy::validate(const std::string &value) const
{
	if(value.empty())
        return false;
    std::size_t semicolon = value.find(";");
    std::size_t space = value.find(" ");
    if (semicolon > value.size())
        return false;
    if (space > semicolon)
	{
		std::string code = value.substr(0, semicolon);
        return (oneParameter(code));
	}
	else
	{
		std::string code = value.substr(0, space);
		std::string direction = value.substr(space + 1, semicolon - space - 1);
		return (twoParameter(code, direction));
	}
}
