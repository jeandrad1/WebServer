#include <fstream>
#include <sstream>
#include "../../../include/parser/config/ServerBlock.hpp"
#include "../../../include/parser/config/Directive.hpp"
#include "../../../include/parser/config/HttpBlock.hpp"
#include "../../../include/parser/config/LocationBlock.hpp"

void buildConfigLine(std::ifstream &filename, std::string &line);

static void processDirective(const std::string &line, AConfigBlock &block)
{
	std::istringstream iss(line);
	std::string key, value;
	if (iss >> key)
	{
		std::size_t semicolon = line.find(";");
		if (semicolon < line.size())
		{
			std::getline(iss, value, ';');
			value.erase(0, value.find_first_not_of(" \t"));
			value.push_back(';');
		}
		else
			std::getline(iss, value);
		block.addBlock(new Directive(key, value));
	}
}

static AConfigBlock *createAndAddBlock(const std::string &type, AConfigBlock &block, std::ifstream &filename)
{
    AConfigBlock *newBlock = NULL;
    
    if (type == "server")
        newBlock = new ServerBlock(type);
    else if (type == "http")
        newBlock = new HttpBlock(type);
    else if (type == "location")
        newBlock = new LocationBlock(type);
    if (newBlock)
    {
        block.addBlock(newBlock);
        createBlock(filename, *newBlock);
    }
    return newBlock;
}

AConfigBlock *createBlock(std::ifstream &filename, AConfigBlock &block)
{
	std::string line;
	buildConfigLine(filename, line);
	while (!line.empty())
	{
		std::size_t http = line.find("http");
		std::size_t semicolon = line.find(";");
		if (line.empty())
			continue;
		if (line.find("server") != std::string::npos && line.find("server_name") == std::string::npos)
			createAndAddBlock("server", block, filename);
		else if (http != std::string::npos && http < line.find(" "))
			createAndAddBlock("http", block, filename);
		else if (line.find("location") != std::string::npos)
			createAndAddBlock("location", block, filename);
		else if (line.find("}") != std::string::npos)
			return (&block);
		else
			processDirective(line, block);
		buildConfigLine(filename, line);
	}
	return (&block);
}
