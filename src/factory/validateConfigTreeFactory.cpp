#include "../strategy/IValidationStrategy.hpp"
#include "../strategy/IValidationStrategyBlock.hpp"
#include "../composite/AConfigBlock.hpp"
#include "../composite/Directive.hpp"
#include "StrategyFactory.hpp"

bool	validateDirectiveCase(AConfigBlock::iterator it, Directive *directive);
bool	validateBlockCase(AConfigBlock::iterator it);

int	validateConfigTreeFactory(AConfigBlock &config)
{
	std::vector<AConfigBlock *>::iterator	ite = config.end();

	int	errorCounter = 0;
	for (std::vector<AConfigBlock *>::iterator it = config.begin(); it != ite; ++it)
	{
		if (Directive *directive = dynamic_cast<Directive *>(*it))
		{
			if (!validateDirectiveCase(it, directive))
				errorCounter++;
		}
		else
		{
			if (!validateBlockCase(it))
				errorCounter++;

			errorCounter += validateConfigTreeFactory(**it);
		}
	}
	return (errorCounter);
}

bool	validateDirectiveCase(AConfigBlock::iterator it, Directive *directive)
{
	IValidationStrategy	*strategy = StrategyFactory::getInstance().chooseStrategy(dynamic_cast<Directive *>(*it)->getName());
	if (strategy)
	{
		if (!strategy->validate(directive->getValue()))
		{
			std::cout << "Error detected for directive " << directive->getName() << " with value " << directive->getValue() << "\n";
			delete strategy;
			return (false);
		}
		delete strategy;
	}
	else
	{
		std::cout << "Error: Invalid directive " << directive->getName() << "\n";
		return (false);
	}
	return (true);
}

bool	validateBlockCase(AConfigBlock::iterator it)
{
	IValidationStrategyBlock	*strategyBlock = StrategyFactory::getInstance().chooseStrategyBlock((*it)->getName());
	if (strategyBlock)
	{
		if (!strategyBlock->validate((*it)->getBlocksVector()))
		{
			std::cout << "Error detected for block " << (*it)->getName() << "\n";
			delete strategyBlock;
			return (false);
		}
		delete strategyBlock;
	}
	else
	{
		std::cout << "Error: Invalid block " << (*it)->getName() << "\n";
		return (false);
	}
	return (true);
}
