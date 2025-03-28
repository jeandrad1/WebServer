#include "../../../include/strategy/strategies/ReturnStrategy.hpp"
#include <vector>
//#include "../../../include/utils/colors.hpp"

static bool isCode(std::string const &value)
{
    std::vector<std::string> valid = \
    {"301", "302", "303", "307", "308", \
    "200", "204", "206", \
    "400", "401", "403", "404", "405", "410", \
    "500", "502", "503", "504"};
    if(value.size() != 3)
        return false;
    for(int i = 0; i < value.size(); i++)
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

/* int main()
{
    ReturnStrategy rs;

    std::string test1 = "200 https://example.com;";
    std::string test2 = "404;";                   
	std::string test3 = "500 https://example.com";
	std::string test4 = "500 https://example.com;";
    std::string test5 = "patata;";                 
    std::string test6 = "999 https://example.com;";

    std::cout << "Test 1: " << (rs.validate(test1) ? GREEN"true" : RED "false") << WHITE << std::endl;
    std::cout << "Test 2: " << (rs.validate(test2) ? GREEN"true" : RED "false") << WHITE << std::endl;
    std::cout << "Test 3: " << (rs.validate(test3) ? RED"true" : GREEN "false") << WHITE << std::endl;
    std::cout << "Test 4: " << (rs.validate(test4) ? GREEN"true" : RED "false") << WHITE << std::endl;
    std::cout << "Test 5: " << (rs.validate(test5) ? RED"true" : GREEN "false") << WHITE << std::endl;
	std::cout << "Test 6: " << (rs.validate(test6) ? RED"true" : GREEN "false") << WHITE << std::endl;

    return 0;
} */