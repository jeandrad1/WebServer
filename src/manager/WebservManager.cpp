#include "WebservManager.hpp"


#include <fstream>
#include "../composite/ServerBlock.hpp"
#include "../factory/StrategyFactory.hpp"

void printBuiltConfigs(const std::vector<void *> &builtConfigs);
std::vector<void *> BuildConfig(AConfigBlock *config_ptr);


AConfigBlock	*createBlock(std::ifstream &filename, AConfigBlock &block);
int				factoryCheck(AConfigBlock &config);
void			registerAllStrategies(StrategyFactory factory);
void			registerBlockStrategies(StrategyFactory factory);
/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

WebservManager::WebservManager(const std::string &configPath) : _configFile(configPath.c_str())
{
    if (!_configFile.is_open())
    {
        std::cerr << "Error: Could not open the file.\n";
    }

    registerAllStrategies(StrategyFactory::getInstance());
	registerBlockStrategies(StrategyFactory::getInstance());
}

WebservManager::~WebservManager(void)
{
    this->_configFile.close();
}

/***********************************************************************/
/*                         Operator Overload                           */
/***********************************************************************/

/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

void WebservManager::run(void)
{
	ServerBlock		config("Config");
	this->_rootBlock = createBlock(this->_configFile, config);

	this->_rootBlock->getBlock(0)->printConfig(0);

	int error = factoryCheck((*this->_rootBlock));

	std::cout << "Passes factory"<< std::endl;
	std::vector<void *> builtConfigs = BuildConfig(this->_rootBlock);
	std::cout << "Passes builder"<< std::endl;
	//printBuiltConfigs(builtConfigs);
}

/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/
