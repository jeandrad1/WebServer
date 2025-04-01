#ifndef IVALIDATIONSTRATEGYBLOCK_HPP
# define IVALIDATIONSTRATEGYBLOCK_HPP

# include "../composite/AConfigBlock.hpp"
# include <algorithm>

class IValidationStrategyBlock {
	protected:
		std::vector<std::string> validateBlock;
	public:
		IValidationStrategyBlock() : validateBlock() {}
		virtual ~IValidationStrategyBlock() {}
		virtual bool validate(std::vector<AConfigBlock*> const &block) const = 0;
};

#endif