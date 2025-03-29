#include <fstream>
#include "../include/parser/config/ServerBlock.hpp"

AConfigBlock *createBlock(std::ifstream &filename, AConfigBlock &block);

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
