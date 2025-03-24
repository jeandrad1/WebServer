#include "../../include/pattern/composite.hpp"

bool isFirstMatch(std::size_t const &targetValue, std::size_t const &first,
	std::size_t const &second, std::size_t const &third)
{
    return (targetValue <= first && targetValue <= second && targetValue <= third);
}

void lineBuilder(std::ifstream &filename, std::string &line)
{
    std::string static buffer;

	if (buffer.empty())
	{
		if (!std::getline(filename, buffer))
		{
			line.clear();
			buffer.clear();
			return ;
		}
	}
	std::size_t semicolon = buffer.find(';');
	std::size_t curlyBracketOpen = buffer.find('{');
	std::size_t curlyBracketClose = buffer.find('}');
    std::size_t hash = buffer.find('#');
    if (curlyBracketClose < buffer.size() && isFirstMatch(curlyBracketClose, curlyBracketOpen, semicolon, hash))
    {
        line = buffer.substr(0, curlyBracketClose + 1);
		buffer = buffer.substr(curlyBracketClose + 1);
    }
    else if (curlyBracketOpen < buffer.size() && isFirstMatch(curlyBracketOpen, curlyBracketClose, semicolon, hash))
    {
        line = buffer.substr(0, curlyBracketOpen + 1);
        buffer = buffer.substr(curlyBracketOpen + 1);
    }
	else if(semicolon < buffer.size() && isFirstMatch(semicolon, curlyBracketClose, curlyBracketOpen, hash))
	{
		line = buffer.substr(0, semicolon + 1);
		buffer = buffer.substr(semicolon + 1);
	}
    else if (hash < buffer.size() && isFirstMatch(hash, curlyBracketClose, curlyBracketOpen, semicolon))
    {
        line = buffer.substr(0, hash);
        buffer.clear();
    }
	else
	{
		line = buffer;
		buffer.clear();
	}
	line.erase(0, line.find_first_not_of(" \t"));
	line.erase(line.find_last_not_of(" \t") + 1);
	if (line == "" || line == "{")
		lineBuilder(filename, line);
}