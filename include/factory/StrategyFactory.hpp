#ifndef STRATEGYFACTORY_HPP
# define STRATEGYFACTORY_HPP

# include "IValidationStrategy.hpp"
# include <map>
# include <string>

typedef IValidationStrategy *(*StrategyCreateFuncPtr)();

class StrategyFactory {

	private:
		std::map<std::string, StrategyCreateFuncPtr> strategyMap;

	public:
		static StrategyFactory &getInstance(void);

		void registerStrategy(const std::string &name, StrategyCreateFuncPtr func);

		IValidationStrategy	*chooseStrategy(const std::string &name);
};

#endif