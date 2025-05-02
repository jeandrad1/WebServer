#include "WebservManager.hpp"


#include <fstream>
#include "../composite/ServerBlock.hpp"
#include "../factory/StrategyFactory.hpp"

void					printBuiltConfigs(const std::vector<IConfig *> &builtConfigs);
std::vector<IConfig *>	createConfigClasses(AConfigBlock *config_ptr);


AConfigBlock			*createBlock(std::ifstream &filename, AConfigBlock &block);
int						validateConfigTreeFactory(AConfigBlock &config);
void					registerAllStrategies(StrategyFactory factory);
void					registerBlockStrategies(StrategyFactory factory);

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
	
	std::vector<IConfig * >::const_iterator ite = this->builtConfigs.end();
	int i = 0;
	for (std::vector<IConfig *>::const_iterator it = this->builtConfigs.begin(); it != ite; ++it)
	{
		delete (*it);
	}
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

	int error = validateConfigTreeFactory((*this->_rootBlock));

	this->builtConfigs = createConfigClasses(this->_rootBlock);

	printBuiltConfigs(this->builtConfigs);
}

/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/
