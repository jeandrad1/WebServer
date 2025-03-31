#include "../../include/strategy/IValidationStrategy.hpp"
#include "../../include/strategy/IValidationStrategyBlock.hpp"
#include "../../include/parser/config/AConfigBlock.hpp"
#include "../../include/parser/config/Directive.hpp"
#include "../../include/factory/StrategyFactory.hpp"

bool	factoryCheckDirectiveCase(AConfigBlock::iterator it, Directive *directive);
bool	factoryCheckBlockCase(AConfigBlock::iterator it);

int factoryCheck(AConfigBlock &config)
{
	std::vector<AConfigBlock *>::iterator ite = config.end();

	int	errorLog = 0;
	for (std::vector<AConfigBlock *>::iterator it = config.begin(); it != ite; ++it)
	{
		if (Directive *directive = dynamic_cast<Directive *>(*it))
		{
			if (!factoryCheckDirectiveCase(it, directive))
				errorLog++;
		}
		else
		{
			if (!factoryCheckBlockCase(it))
				errorLog++;

			errorLog += factoryCheck(**it);
		}
	}
	return (errorLog);
}

bool	factoryCheckDirectiveCase(AConfigBlock::iterator it, Directive *directive)
{
	IValidationStrategy *strategy = StrategyFactory::getInstance().chooseStrategy(dynamic_cast<Directive *>(*it)->getName());
	if (strategy)
	{
		if (!strategy->validate(directive->getValue()))
		{
			std::cout << "Error detected for directive " << directive->getName() << " with value " << directive->getValue() << "\n";
			return (false);
		}
	}
	else
	{
		std::cout << "Error: Invalid directive " << directive->getName() << "\n";
		return (false);
	}
	return (true);
}

bool	factoryCheckBlockCase(AConfigBlock::iterator it)
{
	IValidationStrategyBlock *strategyBlock = StrategyFactory::getInstance().chooseStrategyBlock((*it)->getName());
	if (strategyBlock)
	{
		if (!strategyBlock->validate((*it)->getBlocksVector()))
		{
			std::cout << "Error detected for block " << (*it)->getName() << "\n";
			return (false);
		}
	}
	else
	{
		std::cout << "Error: Invalid block " << (*it)->getName() << "\n";
		return (false);
	}
	return (true);
}