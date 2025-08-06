#include <fstream>
#include "./composite/ServerBlock.hpp"
#include "./factory/StrategyFactory.hpp"
#include "./manager/WebservManager.hpp"
#include "./execute/execute.hpp"

AConfigBlock	*createBlock(std::ifstream &filename, AConfigBlock &block);
int				factoryCheck(AConfigBlock &config);
void			registerAllStrategies(StrategyFactory factory);
void			registerBlockStrategies(StrategyFactory factory);

int	main(int argc, char **argv)
{
	try
	{
		if (argc != 2)
		{
			std::cerr << "Error: Invalid number of arguments\n";
			return (1);
		}

		WebservManager webserv(argv[1]);
		webserv.run();
		
		//int error = factoryCheck(*config_ptr);
		//file.close();
		return (0); // No memory leaks since we used `config` (automatic variable)
	}
	catch (const std::exception&e)
	{
		std::cerr << "Config Error: "<<e.what()<<std::endl;
		return 1;
	}
}
