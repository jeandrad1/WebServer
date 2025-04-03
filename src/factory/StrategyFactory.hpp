#ifndef STRATEGYFACTORY_HPP
# define STRATEGYFACTORY_HPP

# include "../strategy/IValidationStrategy.hpp"
# include "../strategy/IValidationStrategyBlock.hpp"
# include <map>
# include <string>

typedef IValidationStrategy			*(*StrategyCreateFuncPtr)();
typedef IValidationStrategyBlock	*(*StrategyBlockCreateFuncPtr)();

class StrategyFactory {

	private:
		std::map<std::string, StrategyCreateFuncPtr>		strategyMap;
		std::map<std::string, StrategyBlockCreateFuncPtr>	strategyBlockMap;

	public:
		static StrategyFactory &getInstance(void);

		void registerStrategy(const std::string &name, StrategyCreateFuncPtr func);
		void registerStrategy(const std::string &name, StrategyBlockCreateFuncPtr func);

		IValidationStrategy			*chooseStrategy(const std::string &name);
		IValidationStrategyBlock	*chooseStrategyBlock(const std::string &name);
};

#endif
