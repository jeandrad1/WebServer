#include <fstream>
#include <sstream>
#include <iostream>
#include "../include/config/AConfigBlock.hpp"
#include "../include/config/ServerBlock.hpp"
#include "../include/config/Directive.hpp"
#include "../include/config/HttpBlock.hpp"
#include "../include/config/LocationBlock.hpp"

AConfigBlock *createBlock(std::ifstream &filename, AConfigBlock &block)
{
    std::string line;
    while (std::getline(filename, line))
    {
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line.empty()) continue;

        if (line.find("server {") != std::string::npos)
        {
            ServerBlock *server = new ServerBlock("server");
            block.addBlock(server);
            createBlock(filename, *server);
        }
        else if (line.find("http {") != std::string::npos)
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
        {
            return (&block);
        }
        else
        {
            std::istringstream iss(line);
            std::string key, value;
            if (iss >> key)
            {
                std::getline(iss, value, ';'); // Read the rest of the line until semicolon
                value.erase(0, value.find_first_not_of(" \t")); // Trim leading spaces
                if (!value.empty() && value[value.size() - 1] == ';')
                {
                    value.erase(value.size() - 1); // Remove trailing semicolon
                }
                std::cout << "Key: " << key << ", Value: " << value << std::endl;
                block.addBlock(new Directive(key, value));
            }
        }
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