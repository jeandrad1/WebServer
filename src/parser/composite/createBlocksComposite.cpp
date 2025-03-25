#include <fstream>
#include <sstream>
#include "../../include/parser/config/ServerBlock.hpp"
#include "../../include/parser/config/Directive.hpp"
#include "../../include/parser/config/HttpBlock.hpp"
#include "../../include/parser/config/LocationBlock.hpp"

void buildConfigLine(std::ifstream &filename, std::string &line);

AConfigBlock *createBlock(std::ifstream &filename, AConfigBlock &block)
{
    std::string line;
    buildConfigLine(filename, line);
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
				std::getline(iss, value, ';');
                value.erase(0, value.find_first_not_of(" \t"));
				value.push_back(';');
                block.addBlock(new Directive(key, value));
            }
        }
		buildConfigLine(filename, line);
    }
    return (&block);
}
