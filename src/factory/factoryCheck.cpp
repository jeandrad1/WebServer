#include "../../include/parser/config/AConfigBlock.hpp"
#include "../../include/strategy/IValidationStrategy.hpp"
#include "../../include/strategy/IBlockValidationStrategy.hpp"
#include "../../include/parser/config/Directive.hpp"
#include "../../include/factory/StrategyFactory.hpp"

void	factoryCheck(AConfigBlock &config)
{
	std::vector<AConfigBlock *>::iterator ite = config.end();
	for (std::vector<AConfigBlock *>::iterator it = config.begin(); it != ite; ++it)
	{
		//std::cout << (*it)->getName() << "\n";
		if (Directive *directive = dynamic_cast<Directive *>(*it))
		{
			IValidationStrategy *strategy = StrategyFactory::getInstance().chooseStrategy(dynamic_cast<Directive*>(*it)->getDirective());
			if (strategy)
			{
				if (!strategy->validate(directive->getValue()))
					std::cout << "Error detected for directive " << directive->getDirective() << " with value " << directive->getValue() << "\n"; 
			}
			else
				std::cout << "Error: Invalid directive " << directive->getDirective() << "\n";
		}
		else
		{
			std::cout << "Config block detected\n";
			factoryCheck(**it);
		}
	}
}