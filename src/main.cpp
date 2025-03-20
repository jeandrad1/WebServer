#include <fstream>
#include <sstream>
#include <iostream>
#include "../include/utils/colors.hpp"
#include "../include/config/AConfigBlock.hpp"
#include "../include/config/ServerBlock.hpp"
#include "../include/config/Directive.hpp"
#include "../include/config/HttpBlock.hpp"
#include "../include/config/LocationBlock.hpp"

bool isFirstMatch(std::size_t const &targetValue, std::size_t const &first,
    std::size_t const &second, std::size_t const &third)
{
    if (targetValue > first)
        return (false);
    if (targetValue > second)
        return (false);
    if (targetValue > third)
        return (false);
    return (true);
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
	if (line == "")
		lineBuilder(filename, line);
}

AConfigBlock *createBlock(std::ifstream &filename, AConfigBlock &block)
{
    std::string line;
    lineBuilder(filename, line);
    while (!line.empty())
    {
        std::size_t http = line.find("http");
        if (line.empty()) continue;
        if (line.find("server") != std::string::npos && line.find("server_name") == std::string::npos)
        {
			ServerBlock *server = new ServerBlock("server");
            block.addBlock(server);
            createBlock(filename, *server);
        }
        else if (http != std::string::npos && http < line.find(" "))
        {
			HttpBlock *http = new HttpBlock("http");
            block.addBlock(http);
            createBlock(filename, *http);
        }
        else if (line.find("location") != std::string::npos)
        {
            LocationBlock *location = new LocationBlock("location");
            block.addBlock(location);
            createBlock(filename, *location);
        }
        else if (line.find("}") != std::string::npos)
			return (&block);
        else
        {
			std::istringstream iss(line);
            std::string key, value;
            if (iss >> key)
            {
				std::getline(iss, value, ';'); // Read the rest of the line until semicolon
                value.erase(0, value.find_first_not_of(" \t")); // Trim leading spaces
				value.push_back(';');
                block.addBlock(new Directive(key, value));
            }
        }
		lineBuilder(filename, line);
    }
    return (&block);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Error: Invalid number of arguments\n";
        return (1);
    }

    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open the file.\n";
        return (1);
    }
    ServerBlock config("Config");
    AConfigBlock *config_ptr = createBlock(file, config);

    config_ptr->printConfig(0); // Print the parsed configuration
    file.close();
    
    return 0; // No memory leaks since we used `config` (automatic variable)
}