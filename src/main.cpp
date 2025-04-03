#include <fstream>
#include "./composite/ServerBlock.hpp"
#include "./factory/StrategyFactory.hpp"

AConfigBlock	*createBlock(std::ifstream &filename, AConfigBlock &block);
int				factoryCheck(AConfigBlock &config);
void			registerAllStrategies(StrategyFactory factory);
void			registerBlockStrategies(StrategyFactory factory);

int	main(int argc, char **argv)
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

	ServerBlock		config("Config");
	AConfigBlock	*config_ptr = createBlock(file, config);

	registerAllStrategies(StrategyFactory::getInstance());
	registerBlockStrategies(StrategyFactory::getInstance());

	config_ptr->getBlock(0)->printConfig(0); // Print the parsed configuration
	
	int error = factoryCheck(*config_ptr);
	file.close();

	return (0); // No memory leaks since we used `config` (automatic variable)
}
