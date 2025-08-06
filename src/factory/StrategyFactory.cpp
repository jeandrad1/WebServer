#include "StrategyFactory.hpp"

void	registerAllStrategies(StrategyFactory factory);

/***********************************************************************/
/*                     Constructors & Destructor                       */
/***********************************************************************/

/***********************************************************************/
/*                         Operator Overload                           */
/***********************************************************************/


/***********************************************************************/
/*                          Public Functions                           */
/***********************************************************************/

StrategyFactory	&StrategyFactory::getInstance(void)
{
	static StrategyFactory	instance;
	return (instance); 
}

void	StrategyFactory::registerStrategy(const std::string &name, StrategyCreateFuncPtr func)
{
	this->strategyMap[name] = func;
}

void	StrategyFactory::registerStrategy(const std::string &name, StrategyBlockCreateFuncPtr func)
{
	this->strategyBlockMap[name] = func;
}

IValidationStrategy	*StrategyFactory::chooseStrategy(const std::string &name)
{
	if (this->strategyMap.find(name) != strategyMap.end())
	{
		return (strategyMap[name]());
	}
	else
	{
		std::cout << "No strategy detected, name: " << name << "\n";
		return (NULL);
	}
}

IValidationStrategyBlock	*StrategyFactory::chooseStrategyBlock(const std::string &name)
{
	if (this->strategyBlockMap.find(name) != strategyBlockMap.end())
	{
		return (strategyBlockMap[name]());
	}
	else
	{
		if (name.find("location") != std::string::npos)
			return (strategyBlockMap["location"]());
		std::cout << "No strategy detected, name: " << name << "\n";
		return (NULL);
	}
}

/***********************************************************************/
/*                          Getters & Setters                          */
/***********************************************************************/
