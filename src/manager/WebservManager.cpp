#include "WebservManager.hpp"


#include <fstream>
#include "../composite/ServerBlock.hpp"
#include "../factory/StrategyFactory.hpp"
#include "../inheritance/inheritance.hpp"

void printBuiltConfigs(const std::vector<IConfig *> &builtConfigs);
std::vector<IConfig *> buildConfig(AConfigBlock *config_ptr);


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
	InheritanceEngine		heritance;
	this->_rootBlock = createBlock(this->_configFile, config);

	this->_rootBlock->getBlock(0)->printConfig(0);

	int error = factoryCheck((*this->_rootBlock));

	std::cout << "Passes factory"<< std::endl;
	std::vector<IConfig *> builtConfigs = buildConfig(this->_rootBlock);
	std::cout << "Passes builder"<< std::endl;
	heritance.runInherit(builtConfigs);
	printBuiltConfigs(builtConfigs);

	std::vector<IConfig * >::const_iterator ite = builtConfigs.end();
	int i = 0;
    for (std::vector<IConfig *>::const_iterator it = builtConfigs.begin(); it != ite; ++it)
	{
		delete (*it);
	}
}

/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/
