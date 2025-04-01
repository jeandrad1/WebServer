#include <iostream>
#include <fstream>

static bool isFirstMatch(std::size_t const &targetValue, std::size_t const &first,
	std::size_t const &second, std::size_t const &third)
{
    return (targetValue <= first && targetValue <= second && targetValue <= third);
}

static void extractBufferLine(std::string &buffer, std::string &line, std::size_t const &symbol)
{
	line = buffer.substr(0, symbol + 1);
	buffer = buffer.substr(symbol + 1);
	line.erase(0, line.find_first_not_of(" \t"));
	line.erase(line.find_last_not_of(" \t") + 1);
}

static void extractBufferLineAndErase(std::string &buffer, std::string &line, std::size_t const &symbol)
{
	if (symbol != std::string::npos)
		line = buffer.substr(0, symbol);
	else
		line = buffer;
	buffer.clear();
	line.erase(0, line.find_first_not_of(" \t"));
	line.erase(line.find_last_not_of(" \t") + 1);
}

void buildConfigLine(std::ifstream &filename, std::string &line)
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
		extractBufferLine(buffer, line, curlyBracketClose);
    else if (curlyBracketOpen < buffer.size() && isFirstMatch(curlyBracketOpen, curlyBracketClose, semicolon, hash))
		extractBufferLine(buffer, line, curlyBracketOpen);
	else if(semicolon < buffer.size() && isFirstMatch(semicolon, curlyBracketClose, curlyBracketOpen, hash))
		extractBufferLine(buffer, line, semicolon);
    else if (hash < buffer.size() && isFirstMatch(hash, curlyBracketClose, curlyBracketOpen, semicolon))
    	extractBufferLineAndErase(buffer, line, hash);
	else
		extractBufferLineAndErase(buffer, line, std::string::npos);
	if (line == "" || line == "{")
		buildConfigLine(filename, line);
}
