#include "LimitExceptStrategy.hpp"
#include <vector>

std::string	getSubstringBeforeSemicolon(const std::string &value);

std::vector<std::string> split_str(const std::string &str, const std::string &delimiter);

bool	LimitExceptStrategy::validate(const std::string &value) const
{
	if (value.empty())
		return (false);

	if (value[value.size() - 1] != ';')
		return (false);

	std::string	real_value = getSubstringBeforeSemicolon(value);

	size_t		colon_pos = real_value.find(' ');
    if (colon_pos == real_value.size())
        return (false);
    
    std::vector<std::string> values =split_str(real_value," ");

    if (values.size() > 3)
        return (false);

    for (size_t i = 0; i < values.size(); i++)
    {
        if(values[i] != "POST" && values[i] != "GET" && values[i] != "DELETE" )
            return (false);
    }
	return (true);
}
