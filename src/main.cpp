#include <fstream>
#include <sstream>
#include "../include/utils/colors.hpp"
#include "../include/parser/config/AConfigBlock.hpp"
#include "../include/parser/config/ServerBlock.hpp"
#include "../include/parser/config/Directive.hpp"
#include "../include/parser/config/HttpBlock.hpp"
#include "../include/parser/config/LocationBlock.hpp"

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

    config_ptr->getBlock(1)->printConfig(0); // Print the parsed configuration
    
    file.close();
    
    return 0; // No memory leaks since we used `config` (automatic variable)
}
